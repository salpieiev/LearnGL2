//
//  Renderer.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"



struct Programs
{
    GLuint simpleProgram;
    GLuint pointSpriteProgram;
};

struct Attributes
{
    GLint Position;
    GLint SourceColor;
    GLint PointSpritePosition;
};

struct Uniforms
{
    GLuint PointSpriteUniform;
};

struct VertexBuffers
{
    GLuint triangleBuffer;
};

struct Textures
{
    GLuint Star;
};



class Renderer0: public RenderingEngine
{
public:
    Renderer0(int width, int height);
    ~Renderer0();
    
    void Render() const;
    void Update();
    
private:
    void GenTriangleVBO();
    void DrawTriangleWithVBO() const;
    
    void DrawTriangleWithoutVBO() const;
    
    void GenPointSprite();
    void DrawPointSprites() const;
    
    Programs programs;
    Attributes attributes;
    Uniforms uniforms;
    VertexBuffers vertexBuffers;
    Textures textures;
};
