//
//  Renderer6.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer6.h"
#include "Shaders/VertexSkinningShader.vsh"
#include "Shaders/VertexSkinningShader.fsh"
#include "BoneChain.h"



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

struct Skeleton
{
    IndexList Indices;
    VertexList Vertices;
};

struct SkinnedFigure
{
    GLuint IndexBuffer;
    GLuint VertexBuffer;
    MatrixList Matrices;
};



Renderer6::Renderer6(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    m_rotator = new Rotator(m_surfaceSize);
    
    m_program = BuildProgram(VertexSkinningVertexShader, VertexSkinningFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "a_position");
    m_attribSourceColor = glGetAttribLocation(m_program, "a_color");
    m_attribNormal = glGetAttribLocation(m_program, "a_normal");
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
    m_uniformNormalMatrix = glGetUniformLocation(m_program, "u_normalMatrix");
    m_uniformLightPosition = glGetUniformLocation(m_program, "u_lightPosition");
    m_uniformAmbientLight = glGetUniformLocation(m_program, "u_ambientLight");
    m_uniformSpecularLight = glGetUniformLocation(m_program, "u_specularLight");
    m_uniformShininess = glGetUniformLocation(m_program, "u_shininess");
    
    
    glEnableVertexAttribArray(m_attribPosition);
    
    // Create surface
    BoneChain chain;
    chain.AddBone(Bone(vec3(0.5f, 0.5f, 0.0f)));
    chain.AddBone(Bone(vec3(0.5f, -0.5f, 0.0f)));
    chain.AddBone(Bone(vec3(0.0f, 0.0f, 0.5f)));
    chain.AddBone(Bone(vec3(0.5f, -0.5f, 0.0f)));
    chain.AddBone(Bone(vec3(0.5f, -0.5f, -0.5f)));
    chain.AddBone(Bone(vec3(0.5f, -0.5f, 0.0f)));
    
    vector<float> boneData = chain.GetVertexData();
    m_boneVertexCount = chain.GetVertexCount();
    
    // Generate bone vertex buffer
    glGenBuffers(1, &m_boneVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_boneVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boneData.size(), &boneData[0], GL_STATIC_DRAW);
    
    // Set frustum
    GLfloat h = 4 * height / width;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    // Setup uniforms
    glUniform3f(m_uniformLightPosition, 0.25f, 0.25f, 1.0f);
    glUniform3f(m_uniformAmbientLight, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformShininess, 50);
}

Renderer6::~Renderer6()
{
    delete m_rotator;
    delete m_surface;
}

void Renderer6::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::LookAt(Eye, Target, Up);
    mat4 modelview = orientation * translation;
    mat3 normalMatrix = modelview.ToMat3();
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    glVertexAttrib4f(m_attribSourceColor, 1.0f, 0.0f, 0.0f, 1.0f);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    
    glDrawArrays(GL_LINE_STRIP, 0, m_boneVertexCount);
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

void Renderer6::ComputeMatrices(const Skeleton &skeleton, const MatrixList &matrices)
{
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::LookAt(Eye, Target, Up);
    mat4 modelview = orientation * translation;
    
    float x = 0.0f;
    IndexList::const_iterator lineIndex = skeleton.Indices.begin();
    
}


