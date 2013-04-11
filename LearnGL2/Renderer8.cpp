//
//  Renderer8.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 2/28/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer8.h"
#include "Shaders/FogShader.vsh"
#include "Shaders/FogShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
};



Renderer8::Renderer8(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    
    BuildFogProgram();
    
    GenerateCubeData();
    
    ApplyFrustum();
    
    SetupUniforms();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

Renderer8::~Renderer8()
{
    
}

void Renderer8::Render() const
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 modelview;
    
    for (float x = -2.0f; x <= 2.0f; x++) {
        for (float y = -3.0f; y <= 3.0f; y++) {
            for (float z = -3.0f; z <= 2.0f; z++) {
                modelview = modelview.Translate(x, y, -7 + z);
                glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
                
                DrawCube();
            }
        }
    }
}

void Renderer8::OnFingerDown(ivec2 location)
{
    
}

void Renderer8::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer8::OnFingerUp(ivec2 location)
{
    
}

void Renderer8::BuildFogProgram()
{
    m_fogProgram = BuildProgram(FogVertexShader, FogFragmentShader);
    glUseProgram(m_fogProgram);
    
    m_attribPosition = glGetAttribLocation(m_fogProgram, "a_position");
    m_attribColor = glGetAttribLocation(m_fogProgram, "a_color");
    
    m_uniformProjection = glGetUniformLocation(m_fogProgram, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_fogProgram, "u_modelview");
    m_uniformEyePosition = glGetUniformLocation(m_fogProgram, "u_eyePosition");
    m_uniformClipPlane = glGetUniformLocation(m_fogProgram, "u_clipPlane");
    m_uniformFogColor = glGetUniformLocation(m_fogProgram, "u_fogColor");
    m_uniformFogMaxDist = glGetUniformLocation(m_fogProgram, "u_fogMaxDist");
    m_uniformFogMinDist = glGetUniformLocation(m_fogProgram, "u_fogMinDist");
}

void Renderer8::GenerateCubeData()
{
    const Vertex cubeVertices[] =
    {
        {{-0.3f, -0.3f, -0.3f}, {0.0f, 0.0f, 0.0f, 1.0f}},
        {{0.3f, -0.3f, -0.3f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        {{0.3f, 0.3f, -0.3f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.3f, 0.3f, -0.3f}, {0.0f, 1.0f, 1.0f, 1.0f}},
        
        {{-0.3f, -0.3f, 0.3f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.3f, -0.3f, 0.3f}, {1.0f, 0.0f, 1.0f, 1.0f}},
        {{0.3f, 0.3f, 0.3f}, {1.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.3f, 0.3f, 0.3f}, {1.0f, 1.0f, 1.0f, 1.0f}}
    };
    
    const GLubyte cubeIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
        1, 2, 5,
        5, 2, 6,
        3, 2, 7,
        2, 7, 6,
        3, 4, 0,
        3, 4, 7,
        0, 5, 4,
        0, 1, 5,
        4, 7, 5,
        5, 7, 6
    };
    
    m_cubeIndexCount = sizeof(cubeIndices) / sizeof(cubeIndices[0]);
    
    glGenBuffers(1, &m_cubeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_cubeIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
}

void Renderer8::DrawCube() const
{
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribColor);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position)));
    
    glDrawElements(GL_TRIANGLES, m_cubeIndexCount, GL_UNSIGNED_BYTE, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribColor);
}

void Renderer8::ApplyFrustum() const
{
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

void Renderer8::SetupUniforms() const
{
    glUniform4f(m_uniformEyePosition, 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform4f(m_uniformClipPlane, -1.0f, -1.0f, -1.0f, 0.5f);
    glUniform4f(m_uniformFogColor, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform1f(m_uniformFogMinDist, 4.0f);
    glUniform1f(m_uniformFogMaxDist, 10.0f);
}


