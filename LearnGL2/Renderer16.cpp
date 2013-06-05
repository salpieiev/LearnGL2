//
//  Renderer16.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 05.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer16.h"
#include "Shaders/CrudeBloomTextureShader.vsh"
#include "Shaders/CrudeBloomTextureShader.fsh"



Renderer16::Renderer16(int width, int height): RenderingEngine(width, height)
{
    BuildTextureProgram();
    LoadTexture();
}

Renderer16::~Renderer16()
{
    
}

void Renderer16::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawTexture();
}

void Renderer16::OnFingerDown(ivec2 location)
{
    
}

void Renderer16::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer16::OnFingerUp(ivec2 location)
{
    
}

void Renderer16::BuildTextureProgram()
{
    m_textureProgram = BuildProgram(CrudeBloomTextureVertexShader, CrudeBloomTextureFragmentShader);
    glUseProgram(m_textureProgram);
    
    m_attribTexturePosition = glGetAttribLocation(m_textureProgram, "a_position");
    m_attribTextureCoord = glGetAttribLocation(m_textureProgram, "a_texCoord");
}

void Renderer16::LoadTexture()
{
    glGenTextures(1, &m_backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SetPngPOTTexture("bloom_background.png");
}

void Renderer16::DrawTexture() const
{
    GLfloat positions[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
    
    GLfloat texCoords[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    
    glUseProgram(m_textureProgram);
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    
    glEnableVertexAttribArray(m_attribTexturePosition);
    glEnableVertexAttribArray(m_attribTextureCoord);
    
    glVertexAttribPointer(m_attribTexturePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, positions);
    glVertexAttribPointer(m_attribTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, texCoords);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(m_attribTexturePosition);
    glDisableVertexAttribArray(m_attribTextureCoord);
}