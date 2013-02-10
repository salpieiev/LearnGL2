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



#define STRINGIFY(A)    #A



class RenderingEngine
{
public:
    RenderingEngine(int width, int height);
    virtual ~RenderingEngine();
    
    virtual void Render() const = 0;
    virtual void OnFingerDown(ivec2 location) = 0;
    virtual void OnFingerMove(ivec2 oldLocation, ivec2 newLocation) = 0;
    virtual void OnFingerUp(ivec2 location) = 0;
    
protected:
    GLuint BuildShader(const char *source, GLenum shaderType) const;
    GLuint BuildProgram(const char *vertexShader, const char *fragmentShader) const;
    void SetPngPOTTexture(const string &name) const;
    void SetPngNPOTTexture(const string &name) const;
    void SetPVRTexture(const string &name) const;
    
    ivec2 m_surfaceSize;
    ResourceManager *m_resourceManager;
};



void SALog(const char *formatStr, ...);
