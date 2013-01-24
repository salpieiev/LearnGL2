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
    m_trackballRadius = (float)width / 3.0f;
    m_centerPoint = m_surfaceSize / 2;
    
    m_program = BuildProgram(ProjectionVertexShader, ProjectionFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribSourceColor = glGetAttribLocation(m_program, "SourceColor");
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    
    m_surface = new Sphere(0.5f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices);
    
    vector<unsigned short> indices;
    m_surface->GenerateLineIndices(indices);
}

Renderer1::~Renderer1()
{
    delete m_surface;
}

void Renderer1::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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


