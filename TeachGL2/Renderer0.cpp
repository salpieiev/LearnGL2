//
//  Renderer.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer0.h"



#define STRINGIFY(A)    #A
#include "Shaders/Shader.vsh"
#include "Shaders/Shader.fsh"
#include "Shaders/PointSpriteShader.vsh"
#include "Shaders/PointSpriteShader.fsh"



Renderer0::Renderer0(int width, int height): RenderingEngine(width, height)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, width, height);
    
    programs.simpleProgram = BuildProgram(VertexShader, FragmentShader);
    programs.pointSpriteProgram = BuildProgram(PointSpriteVertexShader, PointSpriteFragmentShader);
    
    glUseProgram(programs.simpleProgram);
    attributes.Position = glGetAttribLocation(programs.simpleProgram, "Position");
    attributes.SourceColor = glGetAttribLocation(programs.simpleProgram, "SourceColor");
    glEnableVertexAttribArray(attributes.Position);
    
    glUseProgram(programs.pointSpriteProgram);
    attributes.PointSpritePosition = glGetAttribLocation(programs.pointSpriteProgram, "Position");
    uniforms.PointSpriteUniform = glGetUniformLocation(programs.pointSpriteProgram, "Sampler");
    glEnableVertexAttribArray(attributes.PointSpritePosition);
    
    GenTriangleVBO();
    GenPointSprite();
}

Renderer0::~Renderer0()
{
    
}

void Renderer0::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawTriangleWithoutVBO();
    
    DrawTriangleWithVBO();
    
    DrawPointSprites();
}

void Renderer0::OnFingerDown(ivec2 location)
{
    
}

void Renderer0::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer0::OnFingerUp(ivec2 location)
{
    
}

void Renderer0::GenTriangleVBO()
{
    GLfloat triangleData[] =
    {
        -0.4f, -0.4f, 0.0f,
        -0.25f, 0.25f, 0.0f,
        -0.75f, 0.0f, 0.0f
    };
    
    glGenBuffers(1, &vertexBuffers.triangleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.triangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangleData), triangleData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer0::DrawTriangleWithVBO() const
{
    glUseProgram(programs.simpleProgram);
    
    glVertexAttrib4f(attributes.SourceColor, 1, 0, 0, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.triangleBuffer);
    
    glVertexAttribPointer(attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer0::DrawTriangleWithoutVBO() const
{
    glUseProgram(programs.simpleProgram);
    
    glVertexAttrib4f(attributes.SourceColor, 0.5, 1, 0, 1);
    
    GLfloat vVertices[] =
    {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    
    glVertexAttribPointer(attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer0::GenPointSprite()
{
    TextureDescription textureDescription = m_resourceManager->LoadPngImage("Star.png");
    
    glGenTextures(1, &textures.Star);
    glBindTexture(GL_TEXTURE_2D, textures.Star);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureDescription.size.x, textureDescription.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureDescription.GetImageData());
}

void Renderer0::DrawPointSprites() const
{
    glUseProgram(programs.pointSpriteProgram);
    
    glBindTexture(GL_TEXTURE_2D, textures.Star);
    
    GLfloat spritePositions[] =
    {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f
    };
    
    glVertexAttribPointer(attributes.PointSpritePosition, 3, GL_FLOAT, GL_FALSE, 0, spritePositions);
    
    glEnable(GL_BLEND);
    glDrawArrays(GL_POINTS, 0, 2);
    glDisable(GL_BLEND);
}

