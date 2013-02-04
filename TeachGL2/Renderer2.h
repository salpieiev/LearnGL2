//
//  Renderer2.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"
#include "ParametricSurface.h"
#include "Rotator.h"



class Renderer2: public RenderingEngine
{
public:
    Renderer2(int width, int height);
    ~Renderer2();
    
    void Render() const;
    
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    Rotator *m_rotator;
    ParametricSurface *m_surface;
    
    GLuint m_program;
    
    GLuint m_surfaceVertexBuffer;
    GLuint m_surfaceIndexBuffer;
    GLuint m_surfaceIndexCount;
    
    GLint m_attribPosition;
    GLint m_attribSourceColor;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
};
