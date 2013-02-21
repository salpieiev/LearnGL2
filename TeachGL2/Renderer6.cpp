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
#include "Shaders/SimpleVertexSkinningShader.vsh"
#include "Shaders/SimpleVertexSkinningShader.fsh"



const vec3 Eye(0.0f, 0.0f, 7.0f);
const vec3 Target(0.0f, 0.0f, 0.0f);
const vec3 Up(0, 1, 0);

struct Vertex
{
    vec3 Position;
    vec4 Color;
    vec2 BoneWeights;
    vec2 BoneIndices;
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
}

void Renderer6::PrepareSkinProgram()
{
    m_skinProgram = BuildProgram(VertexSkinningVertexShader, VertexSkinningFragmentShader);
    glUseProgram(m_skinProgram);
    
    m_attribSkinPosition = glGetAttribLocation(m_skinProgram, "a_position");
    m_attribSkinSourceColor = glGetAttribLocation(m_skinProgram, "a_color");
    m_attribSkinBoneWeights = glGetAttribLocation(m_skinProgram, "a_boneWeights");
    m_attribSkinBoneIndices = glGetAttribLocation(m_skinProgram, "a_boneIndices");
    
    m_uniformSkinProjection = glGetUniformLocation(m_skinProgram, "u_projection");
    m_uniformSkinModelview = glGetUniformLocation(m_skinProgram, "u_modelview");
}

void Renderer6::GenerateBoneData()
{
    // Create surface
    m_chain = new BoneChain();
    m_chain->AddBone(Bone(vec3(0.707f, 0.707f, 0.0f)));
    m_chain->AddBone(Bone(vec3(0.707f, -0.707f, 0.0f)));
    m_chain->AddBone(Bone(vec3(-0.707f, -0.707f, 0.0f)));
    m_chain->AddBone(Bone(vec3(1.0f, 0.0f, 0.0f)));
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
    
    int bonesCount = m_chain->GetBones()->size();
    int offset = 0;
    for (int i = 0; i < bonesCount; i++)
    {
        vector<float> vertices;
        vector<GLushort> indices;
        
        if (i == 0) {
            m_skinCylinder->GenerateVertices(vertices, VertexFlagsBoneWeights, ivec2(0, 0));
        } else if (i == bonesCount - 1) {
            m_skinCylinder->GenerateVertices(vertices, VertexFlagsBoneWeights, ivec2(i, i));
        } else {
            m_skinCylinder->GenerateVertices(vertices, VertexFlagsBoneWeights, ivec2(i, i + 1));
        }
        
        m_skinCylinder->GenerateLineIndices(indices);
        
        for (int j = 0; j < vertices.size(); j++) {
            m_skinVertices.push_back(vertices[j]);
        }
        
        for (int j = 0; j < indices.size(); j++) {
            m_skinIndices.push_back(indices[j] + offset);
        }
        
        offset += m_skinCylinder->GetVertexCount();
    }
    
    glGenBuffers(1, &m_skinVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_skinVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_skinVertices.size(), &m_skinVertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_skinIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skinIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_skinIndices.size(), &m_skinIndices[0], GL_STATIC_DRAW);
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
    
    glEnableVertexAttribArray(m_attribBonePosition);
    
    glVertexAttribPointer(m_attribBonePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    
    glDrawArrays(GL_LINE_STRIP, 0, m_chain->GetVertexCount());
    
    glDisableVertexAttribArray(m_attribBonePosition);
}

void Renderer6::DrawSkin() const
{
    glUseProgram(m_skinProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_skinVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skinIndexBuffer);
    
    glEnableVertexAttribArray(m_attribSkinPosition);
    glEnableVertexAttribArray(m_attribSkinSourceColor);
    glEnableVertexAttribArray(m_attribSkinBoneWeights);
    glEnableVertexAttribArray(m_attribSkinBoneIndices);
    
    vector<mat4> rotatedMatices(m_matrices.size());
    for (int i = 0; i < m_matrices.size(); i++)
    {
        mat4 targetMatrix = m_matrices[i];
        
        mat4 orientation = m_rotator->GetOrientation().ToMatrix();
        mat4 translation1 = mat4::Translate(0, 0, -7);
        mat4 translation2 = mat4::Translate(0, 0, 7);
        
        rotatedMatices[i] = translation2 * targetMatrix * orientation * translation1;
    }
    
    glUniformMatrix4fv(m_uniformSkinModelview, rotatedMatices.size(), GL_FALSE, rotatedMatices[0].Pointer());
    
    glVertexAttribPointer(m_attribSkinPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSkinSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position)));
    glVertexAttribPointer(m_attribSkinBoneWeights, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    glVertexAttribPointer(m_attribSkinBoneIndices, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color) + sizeof(Vertex::BoneWeights)));
    
    GLsizei indexCount = m_skinCylinder->GetLineIndexCount() * m_matrices.size();
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribSkinPosition);
    glDisableVertexAttribArray(m_attribSkinSourceColor);
    glDisableVertexAttribArray(m_attribSkinBoneWeights);
    glDisableVertexAttribArray(m_attribSkinBoneIndices);
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
        vec3 b(0.0f, length, 0.0f);
        
        if (offset > 0.0f)
        {
            a.x += offset;
            b.x += offset;
        }
        offset = b.x;
        
        // Compute the matrix that transforms the unflexed bone to its current state
        vec3 B = orientation;
        vec3 A = mat4::RotateZ(90).ToMat3() * orientation;
        vec3 C = A.Cross(B);
        
        mat3 basis(A, B, C);
        vec3 T = (a + b) / 2.0f;
        mat4 rotation = mat4::Translate(-T) * mat4(basis);
        mat4 translation = mat4::Translate(midpoint);
        mat4 resultMatrix = rotation * translation * modelview;
        matrices[boneIndex] = resultMatrix;
    }
}











