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

using namespace std;



struct Programs
{
    GLuint simpleProgram;
};

struct Attributes
{
    GLint Position;
    GLint SourceColor;
};



class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    void Render(int width, int height) const;
    
private:
    GLuint BuildShader(const char *source, GLenum shaderType) const;
    GLuint BuildProgram(const char *vertexShader, const char *fragmentShader) const;
    
    Programs programs;
    Attributes attributes;
};



void SALog(const char *formatStr, ...);