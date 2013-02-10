//
//  Renderer4.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"
#include "ParametricSurface.h"
#include "Rotator.h"



class Renderer4: public RenderingEngine
{
public:
    Renderer4(int width, int height);
    ~Renderer4();
    
    void Render() const;
    
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    Rotator *m_rotator;
    ParametricSurface *m_surface;
    
    GLuint m_program;
    GLint m_attribPosition;
    GLint m_attribSourceColor;
    GLint m_attribNormal;
    GLint m_attribTextureCoord;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    GLuint m_uniformNormalMatrix;
    GLuint m_uniformLightPosition;
    GLuint m_uniformAmbientLight;
    GLuint m_uniformSpecularLight;
    GLuint m_uniformShininess;
    
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_indexCount;
    
    GLuint m_textureFlag;
};
