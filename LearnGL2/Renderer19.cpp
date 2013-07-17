//
//  Renderer19.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 17.07.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer19.h"
#include "Shaders/ProceduralTexturing.vsh"
#include "Shaders/ProceduralTexturing.fsh"



struct Vertex
{
    vec3 Position;
    vec2 TexCoord;
};



Renderer19::Renderer19(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    
    // Program
    GLuint program = BuildProgram(ProceduralTexturingVertexShader, ProceduralTexturingFragmentShader);
    glUseProgram(program);
    
    m_attribPosition = glGetAttribLocation(program, "a_position");
    m_attribTexCoord = glGetAttribLocation(program, "a_texCoord");
}

Renderer19::~Renderer19()
{
    
}

void Renderer19::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLfloat positions[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
    
    GLfloat texCoords[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribTexCoord);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, 0, positions);
    glVertexAttribPointer(m_attribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribTexCoord);
}

void Renderer19::OnFingerDown(ivec2 location)
{
    
}

void Renderer19::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer19::OnFingerUp(ivec2 location)
{
    
}