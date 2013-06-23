//
//  Renderer18.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 22.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer18.h"
#include "ParametricSurface.h"
#include "Shaders/ProjectiveTexturingLightingShader.vsh"
#include "Shaders/ProjectiveTexturingLightingShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
};



Renderer18::Renderer18(int width, int height): RenderingEngine(width, height)
{
    BuildLightingProgram();
    GenerateSurfaceBuffers();
    SetupLightingUniforms();
}

Renderer18::~Renderer18()
{
    
}

void Renderer18::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawSurface();
}

void Renderer18::OnFingerDown(ivec2 location)
{
    
}

void Renderer18::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer18::OnFingerUp(ivec2 location)
{
    
}

void Renderer18::BuildLightingProgram()
{
    m_lightingProgram = BuildProgram(ProjectiveTexturingLightingVertexShader, ProjectiveTexturingLightingFragmentShader);
    glUseProgram(m_lightingProgram);
    
    m_attribLightingPosition = glGetAttribLocation(m_lightingProgram, "a_position");
    m_attribLightingColor = glGetAttribLocation(m_lightingProgram, "a_color");
    
    m_uniformLightingProjection = glGetUniformLocation(m_lightingProgram, "u_projection");
    m_uniformLightingModelview = glGetUniformLocation(m_lightingProgram, "u_modelview");
}

void Renderer18::GenerateSurfaceBuffers()
{
    MobiusStrip surface = MobiusStrip(1.5f);
    
    vector<GLfloat> vertices;
    surface.GenerateVertices(vertices, VertexFlagsColors);
    
    vector<GLushort> indices;
    surface.GenerateLineIndices(indices);
    m_surfaceIndexCount = indices.size();
    
    glGenBuffers(1, &m_surfaceVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surfaceIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surfaceIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
}

void Renderer18::SetupLightingUniforms() const
{
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformLightingProjection, 1, GL_FALSE, projection.Pointer());
}

void Renderer18::DrawSurface() const
{
    glUseProgram(m_lightingProgram);
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    
    mat4 modelview;
    modelview = modelview.Translate(0, 0, -7.0f);
    glUniformMatrix4fv(m_uniformLightingModelview, 1, GL_FALSE, modelview.Pointer());
    
    glEnableVertexAttribArray(m_attribLightingPosition);
    glEnableVertexAttribArray(m_attribLightingColor);
    
    glVertexAttribPointer(m_attribLightingPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribLightingColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position)));
    
    glDrawElements(GL_LINES, m_surfaceIndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribLightingPosition);
    glDisableVertexAttribArray(m_attribLightingColor);
}

















