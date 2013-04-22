//
//  Renderer7.cpp
//  LearnGL2
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
    m_uniformBaseSampler = glGetUniformLocation(m_program, "u_baseSampler");
    m_uniformLightSampler = glGetUniformLocation(m_program, "u_lightSampler");
    
    // Generate back texture
    glActiveTexture(GL_TEXTURE0);
    
    glGenTextures(1, &m_backTexture);
    glBindTexture(GL_TEXTURE_2D, m_backTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SetPngPOTTexture("tile_floor.png");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glUniform1i(m_uniformBaseSampler, 0);
    
    // Generate light texture
    glActiveTexture(GL_TEXTURE1);
    
    glGenTextures(1, &m_lightTexture);
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SetPngPOTTexture("lightmap.png");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glUniform1i(m_uniformLightSampler, 1);
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
        128, 384, 0, 0, 1,
        512, 384, 0, 1, 1,
        512, 768, 0, 1, 0,
        
        128, 384, 0, 0, 1,
        512, 768, 0, 1, 0,
        128, 768, 0, 0, 0
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
    glActiveTexture(GL_TEXTURE0);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 1024, 1024, 0);
}