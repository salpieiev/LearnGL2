//
//  Renderer10.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 4/9/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer10.h"
#include "Shaders/BlendingShader.vsh"
#include "Shaders/BlendingShader.fsh"



Renderer10::Renderer10(int width, int height): RenderingEngine(width, height)
{
    m_program = BuildProgram(BlendingVertexShader, BlendingFragmentShader);
    glUseProgram(m_program);
    
    glViewport(0, 0, width, height);
    
    m_attribPosition = glGetAttribLocation(m_program, "a_position");
    m_attribColor = glGetAttribLocation(m_program, "a_color");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
}

Renderer10::~Renderer10()
{
    
}

void Renderer10::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    mat4 modelview;
    
    // Red Quad
    glVertexAttrib4f(m_attribColor, 1.0f, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    DrawQuad();
    
    // Green Quad
    modelview = modelview.Translate(-0.5f, -0.5f, 0.0f);
    
    glVertexAttrib4f(m_attribColor, 0.0f, 1.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    DrawQuad();
}

void Renderer10::OnFingerDown(ivec2 location)
{
    
}

void Renderer10::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer10::OnFingerUp(ivec2 location)
{
    
}

void Renderer10::DrawQuad() const
{
    vec3 vertices[] =
    {
        {-0.4f, -0.4f, 0.0f},
        {0.4f, -0.4f, 0.0f},
        {0.4f, 0.4f, 0.0f},
        {-0.4f, -0.4f, 0.0f},
        {0.4f, 0.4f, 0.0f},
        {-0.4f, 0.4f, 0.0f},
    };
    
    glEnableVertexAttribArray(m_attribPosition);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), vertices);
    
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
    
    glDisableVertexAttribArray(m_attribPosition);
}


