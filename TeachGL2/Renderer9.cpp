//
//  Renderer9.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 4/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer9.h"
#include "Shaders/StencilShader.vsh"
#include "Shaders/StencilShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
};



Renderer9::Renderer9(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    
    m_rotator = new Rotator(m_surfaceSize);
    m_surface0 = new Sphere(2.0f);
    
    PrepareProgram();
    GenerateBuffers();
    SetupProjection();
}

Renderer9::~Renderer9()
{
    delete m_surface0;
    delete m_surface1;
    delete m_rotator;
}

void Renderer9::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    DrawSphere();
}

void Renderer9::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer9::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer9::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}

void Renderer9::PrepareProgram()
{
    m_stencilProgram = BuildProgram(StencilVertexShader, StencilFragmentShader);
    glUseProgram(m_stencilProgram);
    
    m_attribPosition = glGetAttribLocation(m_stencilProgram, "a_position");
    m_attribColor = glGetAttribLocation(m_stencilProgram, "a_color");
    m_uniformProjection = glGetUniformLocation(m_stencilProgram, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_stencilProgram, "u_modelview");
}

void Renderer9::GenerateBuffers()
{
    vector<GLfloat> vertices;
    m_surface0->GenerateVertices(vertices, VertexFlagsColors);
    
    vector<GLushort> indices;
    m_surface0->GenerateLineIndices(indices);
    m_surface0IndexCount = indices.size();
    
    glGenBuffers(1, &m_surface0VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surface0VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surface0IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surface0IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
}

void Renderer9::SetupProjection() const
{
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

void Renderer9::DrawSphere() const
{
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    
    mat4 modelview;
    modelview = orientation * modelview.Translate(0.0f, 0.0f, -7.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribColor);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    
    glDrawElements(GL_LINES, m_surface0IndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribColor);
}




