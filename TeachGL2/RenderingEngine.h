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
#include "Quaternion.h"
#include "ResourceManager.h"

using namespace std;



class RenderingEngine
{
public:
    RenderingEngine(int width, int height);
    virtual ~RenderingEngine();
    
    virtual void Render() const = 0;
    virtual void Update() = 0;
    
protected:
    GLuint BuildShader(const char *source, GLenum shaderType) const;
    GLuint BuildProgram(const char *vertexShader, const char *fragmentShader) const;
    
    ivec2 surfaceSize;
    ResourceManager *resourceManager;
};



void SALog(const char *formatStr, ...);
