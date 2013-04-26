//
//  Renderer13.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 26.04.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer13.h"
#include "Shaders/LightingShader.vsh"
#include "Shaders/LightingShader.fsh"
#include "ParametricSurface.h"



struct Vertex
{
    vec3 Position;
};



Renderer13::Renderer13(int width, int height): RenderingEngine(width, height)
{
    m_rotator = new Rotator(m_surfaceSize);
    
    glViewport(0, 0, width, height);
    
    PrepareProgram();
    GenerateBuffers();
    SetupUniforms();
}

Renderer13::~Renderer13()
{
    delete m_rotator;
}

void Renderer13::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawSphere();
}

void Renderer13::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer13::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer13::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}

void Renderer13::PrepareProgram()
{
    m_program = BuildProgram(LightingVertexShader, LightingFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "a_position");
    m_attribColor = glGetAttribLocation(m_program, "a_color");
    
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
}

void Renderer13::GenerateBuffers()
{
    Sphere sphere(2.0f);
    
    vector<GLfloat> vertices;
    sphere.GenerateVertices(vertices, VertexFlagsNormals);
    
    vector<GLushort> indices;
    sphere.GenerateTriangleIndices(indices);
    m_indexCount = indices.size();
    
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Renderer13::SetupUniforms() const
{
    GLfloat h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

void Renderer13::DrawSphere() const
{
    mat4 modelview = m_rotator->GetOrientation().ToMatrix();
    modelview = modelview * mat4::Translate(0.0f, 0.0f, -7.0f);
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glVertexAttrib4f(m_attribColor, 1.0f, 0.0f, 0.0f, 1.0f);
    
    glEnableVertexAttribArray(m_attribPosition);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
}