//
//  Renderer7.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 23.02.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"



class Renderer7: public RenderingEngine
{
public:
    Renderer7(int width, int height);
    ~Renderer7();
    
    void Render() const;
    
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    GLuint m_program;
    
    GLuint m_backTexture;
    GLuint m_lightTexture;
    
    GLint m_attribPosition;
    GLint m_attribTexCoord;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    GLuint m_uniformBaseSampler;
    GLuint m_uniformLightSampler;
};