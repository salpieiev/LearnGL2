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
    vec3 Normal;
};



Renderer9::Renderer9(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    m_rotator = new Rotator(m_surfaceSize);
    m_surface0 = new Sphere(2.0f);
    m_surface1 = new MobiusStrip(1.5f);
    
    PrepareProgram();
    GenerateBuffers();
    SetupUniforms();
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
    
    DrawSurface0();
    DrawSurface1();
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
    m_attribNormal = glGetAttribLocation(m_stencilProgram, "a_normal");
    m_uniformProjection = glGetUniformLocation(m_stencilProgram, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_stencilProgram, "u_modelview");
    m_uniformLightPosition = glGetUniformLocation(m_stencilProgram, "u_lightPosition");
    m_uniformNormalMatrix = glGetUniformLocation(m_stencilProgram, "u_normalMatrix");
    m_uniformAmbientLight = glGetUniformLocation(m_stencilProgram, "u_ambientLight");
}

void Renderer9::GenerateBuffers()
{
    vector<GLfloat> surface0vertices;
    m_surface0->GenerateVertices(surface0vertices, VertexFlagsColors | VertexFlagsNormals);
    
    vector<GLushort> surface0indices;
    m_surface0->GenerateTriangleIndices(surface0indices);
    m_surface0IndexCount = surface0indices.size();
    
    glGenBuffers(1, &m_surface0VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surface0VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, surface0vertices.size() * sizeof(GLfloat), &surface0vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surface0IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surface0IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface0indices.size() * sizeof(GLushort), &surface0indices[0], GL_STATIC_DRAW);
    
    
    vector<GLfloat> surface1vertices;
    m_surface1->GenerateVertices(surface1vertices, VertexFlagsColors | VertexFlagsNormals);
    
    vector<GLushort> surface1indices;
    m_surface1->GenerateTriangleIndices(surface1indices);
    m_surface1IndexCount = surface1indices.size();
    
    glGenBuffers(1, &m_surface1VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surface1VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, surface1vertices.size() * sizeof(GLfloat), &surface1vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surface1IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surface1IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface1indices.size() * sizeof(GLushort), &surface1indices[0], GL_STATIC_DRAW);
}

void Renderer9::SetupUniforms() const
{
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    glUniform3f(m_uniformLightPosition, 3.0f, 3.0f, 10.0f);
    glUniform3f(m_uniformAmbientLight, 0.0f, 0.0f, 0.0f);
}

void Renderer9::DrawSurface0() const
{
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    
    mat4 modelview;
    modelview = orientation * modelview.Translate(0.0f, 0.0f, -7.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, modelview.ToMat3().Pointer());
    
    glBindBuffer(GL_ARRAY_BUFFER, m_surface0VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surface0IndexBuffer);
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribColor);
    glEnableVertexAttribArray(m_attribNormal);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    
    glDrawElements(GL_TRIANGLES, m_surface0IndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribColor);
    glDisableVertexAttribArray(m_attribNormal);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer9::DrawSurface1() const
{
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    
    mat4 modelview;
    modelview = orientation * modelview.Translate(0.0f, 0.0f, -7.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, modelview.ToMat3().Pointer());
    
    glBindBuffer(GL_ARRAY_BUFFER, m_surface1VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surface1IndexBuffer);
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribColor);
    glEnableVertexAttribArray(m_attribNormal);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    
    glDrawElements(GL_TRIANGLES, m_surface1IndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribColor);
    glDisableVertexAttribArray(m_attribNormal);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}




