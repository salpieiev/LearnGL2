//
//  Renderer7.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 23.02.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer7.h"
#include "Shaders/MultitextureShader.vsh"
#include "Shaders/MultitextureShader.fsh"



Renderer7::Renderer7(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    
    m_program = BuildProgram(MultitextureVertexShader, MultitextureFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "a_position");
    m_attribTexCoord = glGetAttribLocation(m_program, "a_texCoord");
    
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
    
    // Generate back texture
    glGenTextures(1, &m_backTexture);
    glBindTexture(GL_TEXTURE_2D, m_backTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SetPngPOTTexture("tile_floor.png");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Renderer7::~Renderer7()
{
    
}

void Renderer7::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Setup modelview
    mat4 modelview;
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    // Setup projection
    mat4 projection = mat4::Ortho(0, m_surfaceSize.x, 0, m_surfaceSize.y, -1, 1);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    float quad[] =
    {
        200, 200, 0, 0, 0,
        500, 200, 0, 1, 0,
        500, 500, 0, 1, 1,
        
        200, 200, 0, 0, 0,
        500, 500, 0, 1, 1,
        200, 500, 0, 0, 1
    };
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribTexCoord);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, quad);
    glVertexAttribPointer(m_attribTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, &quad[3]);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribTexCoord);
}

void Renderer7::OnFingerDown(ivec2 location)
{
    
}

void Renderer7::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer7::OnFingerUp(ivec2 location)
{
    
}