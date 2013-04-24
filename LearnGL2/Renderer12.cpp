//
//  Renderer12.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/23/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer12.h"
#include "Shaders/NormalMapping.vsh"
#include "Shaders/NormalMapping.fsh"



Renderer12::Renderer12(int width, int height): RenderingEngine(width, height)
{
    m_surface = new Sphere(1.0f);
    m_rotator = new Rotator(m_surfaceSize);
    
    glViewport(0, 0, width, height);
    
    PrepareProgram();
    
//    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
//    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
}

Renderer12::~Renderer12()
{
    delete m_surface;
    delete m_rotator;
}

void Renderer12::Render() const
{
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer12::OnFingerDown(ivec2 location)
{
    
}

void Renderer12::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer12::OnFingerUp(ivec2 location)
{
    
}

void Renderer12::PrepareProgram()
{
    m_program = BuildProgram(NormalMappingVertexShader, NormalMappingFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "a_position");
    m_attribColor = glGetAttribLocation(m_program, "a_color");
    
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
}




