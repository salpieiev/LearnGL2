//
//  RenderingEngine.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include <iostream>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "Matrix.h"
#include "ResourceManager.h"

using namespace std;



class RenderingEngine
{
public:
    RenderingEngine();
    ~RenderingEngine();
    
    virtual void Render(int width, int height) const = 0;
    virtual void Update() = 0;
    
protected:
    GLuint BuildShader(const char *source, GLenum shaderType) const;
    GLuint BuildProgram(const char *vertexShader, const char *fragmentShader) const;
    
    ResourceManager *resourceManager;
};



void SALog(const char *formatStr, ...);
