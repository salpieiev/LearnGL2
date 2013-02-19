//
//  Renderer6.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer6.h"
#include "Shaders/BoneShader.vsh"
#include "Shaders/BoneShader.fsh"
#include "Shaders/VertexSkinningShader.vsh"
#include "Shaders/VertexSkinningShader.fsh"



const vec3 Eye(0.0f, 0.0f, 7.0f);
const vec3 Target(0.0f, 0.0f, 0.0f);
const vec3 Up(0, 1, 0);

struct Vertex
{
    vec3 Position;
//    vec4 Color;
//    vec3 Normal;
//    vec2 BoneWeights;
//    float Padding0;
//    unsigned short boneIndices;
//    unsigned short Padding1;
};

typedef vector<Vertex> VertexList;
typedef vector<GLushort> IndexList;

struct SkinnedFigure
{
    GLuint IndexBuffer;
    GLuint VertexBuffer;
    vector<mat4> Matrices;
};



Renderer6::Renderer6(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    m_rotator = new Rotator(m_surfaceSize);
    
    PrepareBoneProgram();
    PrepareSkinProgram();
    
    GenerateBoneData();
    GenerateSkinData();
    
    SetupBoneUniforms();
    SetupSkinUniforms();
}

Renderer6::~Renderer6()
{
    delete m_rotator;
    delete m_skinCylinder;
    delete m_chain;
}

void Renderer6::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawBones();
    DrawSkin();
}

void Renderer6::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer6::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer6::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}

void Renderer6::PrepareBoneProgram()
{
    m_boneProgram = BuildProgram(BoneVertexShader, BoneFragmentShader);
    glUseProgram(m_boneProgram);
    
    m_attribBonePosition = glGetAttribLocation(m_boneProgram, "Position");
    m_attribBoneSourceColor = glGetAttribLocation(m_boneProgram, "SourceColor");
    
    m_uniformBoneProjection = glGetUniformLocation(m_boneProgram, "Projection");
    m_uniformBoneModelview = glGetUniformLocation(m_boneProgram, "Modelview");
    
    glEnableVertexAttribArray(m_attribBonePosition);
}

void Renderer6::PrepareSkinProgram()
{
    m_skinProgram = BuildProgram(VertexSkinningVertexShader, VertexSkinningFragmentShader);
    glUseProgram(m_skinProgram);
    
    m_attribSkinPosition = glGetAttribLocation(m_skinProgram, "a_position");
    m_attribSkinSourceColor = glGetAttribLocation(m_skinProgram, "a_color");
    m_attribSkinNormal = glGetAttribLocation(m_skinProgram, "a_normal");
    m_uniformSkinProjection = glGetUniformLocation(m_skinProgram, "u_projection");
    m_uniformSkinModelview = glGetUniformLocation(m_skinProgram, "u_modelview");
    m_uniformSkinNormalMatrix = glGetUniformLocation(m_skinProgram, "u_normalMatrix");
    m_uniformSkinLightPosition = glGetUniformLocation(m_skinProgram, "u_lightPosition");
    m_uniformSkinAmbientLight = glGetUniformLocation(m_skinProgram, "u_ambientLight");
    m_uniformSkinSpecularLight = glGetUniformLocation(m_skinProgram, "u_specularLight");
    m_uniformSkinShininess = glGetUniformLocation(m_skinProgram, "u_shininess");
    
    glEnableVertexAttribArray(m_attribSkinPosition);
}

void Renderer6::GenerateBoneData()
{
    // Create surface
    m_chain = new BoneChain();
    m_chain->AddBone(Bone(vec3(0.707f, 0.707f, 0.0f)));
    m_chain->AddBone(Bone(vec3(0.707f, -0.707f, 0.0f)));
    m_chain->AddBone(Bone(vec3(0.0f, 0.0f, 0.707f)));
    m_chain->AddBone(Bone(vec3(0.707f, -0.707f, 0.0f)));
    m_chain->AddBone(Bone(vec3(0.707f, -0.707f, -0.707f)));
    m_chain->AddBone(Bone(vec3(0.707f, -0.707f, 0.0f)));
    
    ComputeMatrices(m_matrices);
    
    vector<float> boneData = m_chain->GetVertexData();
    
    // Generate bone vertex buffer
    glGenBuffers(1, &m_boneVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_boneVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boneData.size(), &boneData[0], GL_STATIC_DRAW);
}

void Renderer6::GenerateSkinData()
{
    m_skinCylinder = new Cylinder(1.0f, 0.25f);
    
    vector<float> vertices;
    vector<GLushort> indices;
    
    m_skinCylinder->GenerateVertices(vertices);
    m_skinCylinder->GenerateLineIndices(indices);
    
    glGenBuffers(1, &m_skinVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_skinVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_skinIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skinIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Renderer6::SetupBoneUniforms()
{
    glUseProgram(m_boneProgram);
    
    // Set frustum
    GLfloat h = 4 * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformBoneProjection, 1, GL_FALSE, projection.Pointer());
}

void Renderer6::SetupSkinUniforms()
{
    glUseProgram(m_skinProgram);
    
    // Set frustum
    GLfloat h = 4 * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformSkinProjection, 1, GL_FALSE, projection.Pointer());
    
    // Setup uniforms
    glUniform3f(m_uniformSkinLightPosition, 0.25f, 0.25f, 1.0f);
    glUniform3f(m_uniformSkinAmbientLight, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformSkinSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformSkinShininess, 50);
}

void Renderer6::DrawBones() const
{
    glUseProgram(m_boneProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_boneVertexBuffer);
    
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::LookAt(Eye, Target, Up);
    mat4 modelview = orientation * translation;
    
    glUniformMatrix4fv(m_uniformBoneModelview, 1, GL_FALSE, modelview.Pointer());
    glVertexAttrib4f(m_attribBoneSourceColor, 1.0f, 0.0f, 0.0f, 1.0f);
    
    glVertexAttribPointer(m_attribBonePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    
    glDrawArrays(GL_LINE_STRIP, 0, m_chain->GetVertexCount());
}

void Renderer6::DrawSkin() const
{
    glUseProgram(m_skinProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_skinVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skinIndexBuffer);
    
    for (int i = 0; i < m_matrices.size(); i++)
    {
        mat4 orientation = m_rotator->GetOrientation().ToMatrix();
        
        mat4 modelview = orientation * m_matrices[i];
        mat3 normalMatrix = modelview.ToMat3();
//
//        mat4 orientation = m_rotator->GetOrientation().ToMatrix();
//        mat4 translation = mat4::LookAt(Eye, Target, Up);
//        mat4 modelview = orientation * translation;
//        mat3 normalMatrix = modelview.ToMat3();
        
        glUniformMatrix4fv(m_uniformSkinModelview, 1, GL_FALSE, modelview.Pointer());
        glUniformMatrix4fv(m_uniformSkinNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
        glVertexAttrib4f(m_attribSkinSourceColor, 0.0f, 1.0f, 0.0f, 1.0f);
        
        glVertexAttribPointer(m_attribSkinPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
        
        GLsizei indexCount = m_skinCylinder->GetLineIndexCount();
        glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_SHORT, NULL);
    }
}

void Renderer6::ComputeMatrices(vector<mat4> &matrices)
{
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::LookAt(Eye, Target, Up);
    mat4 modelview = orientation * translation;
    
    float offset = 0.0f;
    int bonesCount = m_chain->GetBones()->size();
    matrices.resize(bonesCount);
    
    for (int boneIndex = 0; boneIndex < bonesCount; boneIndex++)
    {
        // Compute the length, orientation and midpoint of this bone
        vec3 start;
        vec3 end;
        
        m_chain->BoneCoordinateAtIndex(boneIndex, start, end);
        
        float length = (end - start).Length();
        vec3 orientation = (end - start) / length;
        vec3 midpoint = (start + end) / 2.0f;
        
        // Find the endpoints of the unflexed bone, that sits at the origin
        vec3 a(0.0f, 0.0f, 0.0f);
        vec3 b(length, 0.0f, 0.0f);
        
        if (offset > 0.0f)
        {
            a.x += offset;
            b.x += offset;
        }
        offset = b.x;
        
        // Compute the matrix that transforms the unflexed bone to its current state
        vec3 A = orientation;
        vec3 B = vec3(-A.y, A.x, 0.0f);
        vec3 C = A.Cross(B);
        
        mat3 basis(A, B, C);
        vec3 T = (a + b) / 2.0f;
        mat4 rotation = mat4::Translate(-T) * mat4(basis);
        mat4 translation = mat4::Translate(midpoint);
        matrices[boneIndex] = rotation * translation * modelview;
    }
}











