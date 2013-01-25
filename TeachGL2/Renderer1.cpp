//
//  Renderer1.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer1.h"

#define STRINGIFY(A)    #A
#include "Shaders/ProjectionShader.vsh"
#include "Shaders/ProjectionShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
};



Renderer1::Renderer1(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    
    m_trackballRadius = (float)width / 3.0f;
    m_centerPoint = m_surfaceSize / 2;
    
    m_program = BuildProgram(ProjectionVertexShader, ProjectionFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribSourceColor = glGetAttribLocation(m_program, "SourceColor");
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribSourceColor);
    
    // Create surface
    m_surface = new Sphere(3.0f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices, VertexFlagsColors);
    
    vector<unsigned short> indices;
    m_surface->GenerateLineIndices(indices);
    m_indexCount = indices.size();
    
    // Generate vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    // Generate index buffer
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
    // Set frustum
    GLfloat h = 4 * height / width;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

Renderer1::~Renderer1()
{
    delete m_surface;
}

void Renderer1::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Modelview
    mat4 modelview;
    modelview = modelview.Translate(0.0f, 0.0f, -7.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    
    glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
}

void Renderer1::OnFingerDown(ivec2 location)
{
    
}

void Renderer1::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer1::OnFingerUp(ivec2 location)
{
    
}

vec3 Renderer1::MapToSphere(ivec2 touchLocation) const
{
    vec2 p = touchLocation - m_centerPoint;
    
    // Flip the y-axis because pixel coords increase toward the bottom
    p.y = -p.y;
    
    const float safeRadius = m_trackballRadius - 1.0f;
    
    if (p.Length() > safeRadius)
    {
        float theta = atan2(p.y, p.x);
        p.x = safeRadius * cos(theta);
        p.y = safeRadius * sin(theta);
    }
    
    float z = sqrtf(m_trackballRadius * m_trackballRadius - p.LengthSquared());
    vec3 mapped = vec3(p.x, p.y, z);
    mapped /= m_trackballRadius;
    
    return mapped;
}


