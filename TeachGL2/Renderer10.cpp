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
    
    // Configure blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Red Quad
    mat4 modelview;
    
    glVertexAttrib4f(m_attribColor, 1.0f, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    DrawQuad();
    
    // Green Quad
    modelview = modelview.Translate(-0.5f, -0.5f, 0.0f);
    
    glVertexAttrib4f(m_attribColor, 0.0f, 1.0f, 0.0f, 0.5f);
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    DrawQuad();
}

void Renderer10::OnFingerDown(ivec2 location)
{
    GLint readType = 0;
    GLint readFormat = 0;
    
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &readType);
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &readFormat);
    
    GLuint bytesPerPixel = 0;
    
    switch (readType)
    {
        case GL_UNSIGNED_BYTE:
        {
            switch (readFormat)
            {
                case GL_BGRA:
                case GL_RGBA:
                {
                    bytesPerPixel = 4;
                    break;
                }
                case GL_RGB:
                {
                    bytesPerPixel = 3;
                    break;
                }
                case GL_LUMINANCE_ALPHA:
                {
                    bytesPerPixel = 2;
                    break;
                }
                case GL_ALPHA:
                case GL_LUMINANCE:
                {
                    bytesPerPixel = 1;
                    break;
                }
            }
            break;
        }
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_5_6_5:
        {
            bytesPerPixel = 2;
            break;
        }
    }
    
    GLubyte *pixels = (GLubyte *)malloc(m_surfaceSize.x * m_surfaceSize.y * bytesPerPixel);
    
    glReadPixels(0, 0, m_surfaceSize.x, m_surfaceSize.y, readFormat, readType, pixels);
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


