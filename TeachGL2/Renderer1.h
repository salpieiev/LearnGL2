//
//  Renderer1.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"
#include "ParametricSurface.h"



class Renderer1: public RenderingEngine
{
public:
    Renderer1(int width, int height);
    ~Renderer1();
    
    void Render() const;
    
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    vec3 MapToSphere(ivec2 touchLocation) const;
    
    float m_trackballRadius;
    bool m_spinning;
    ivec2 m_centerPoint;
    ivec2 m_fingerStart;
    Quaternion m_orientation;
    Quaternion m_prevOrientation;
    
    ParametricSurface *m_surface;
    
    GLuint m_program;
    GLint m_attribPosition;
    GLint m_attribSourceColor;
    GLint m_attribNormal;
    
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
};
