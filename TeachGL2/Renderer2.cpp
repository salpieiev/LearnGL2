//
//  Renderer2.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer2.h"
#include "Shaders/LightShader.vsh"
#include "Shaders/LightShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
};




Renderer2::Renderer2(int width, int height): RenderingEngine(width, height)
{
    m_rotator = new Rotator(m_surfaceSize);
    
    // Create and link program
    m_program = BuildProgram(LightVertexShader, LightFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribSourceColor = glGetAttribLocation(m_program, "SourceColor");
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    
    // Generate VBOs for surface
    m_surface = new Torus(1.8f, 0.5f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices, VertexFlagsColors);
    
    vector<unsigned short> indices;
    m_surface->GenerateLineIndices(indices);
    m_surfaceIndexCount = indices.size();
    
    glGenBuffers(1, &m_surfaceVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surfaceIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surfaceIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

Renderer2::~Renderer2()
{
    delete m_rotator;
    delete m_surface;
}

void Renderer2::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set viewport
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    
    // Apply frustum
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    // Apply modelview
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 rotation = m_rotator->GetOrientation().ToMatrix();
    mat4 modelview = rotation * translation;
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    // Draw surface
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribSourceColor);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    
    glDrawElements(GL_LINES, m_surfaceIndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribSourceColor);
}

void Renderer2::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer2::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer2::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}