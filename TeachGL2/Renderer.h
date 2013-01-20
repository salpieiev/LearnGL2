//
//  Renderer.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <iostream>
#include "ResourceManager.h"

using namespace std;



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



class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    void Render(int width, int height) const;
    void Update();
    
private:
    GLuint BuildShader(const char *source, GLenum shaderType) const;
    GLuint BuildProgram(const char *vertexShader, const char *fragmentShader) const;
    
    void GenTriangleVBO();
    void DrawTriangleWithVBO() const;
    
    void DrawTriangleWithoutVBO() const;
    
    void GenPointSprite();
    void DrawPointSprites() const;
    
    ResourceManager *resourceManager;
    
    Programs programs;
    Attributes attributes;
    Uniforms uniforms;
    VertexBuffers vertexBuffers;
    Textures textures;
};



void SALog(const char *formatStr, ...);