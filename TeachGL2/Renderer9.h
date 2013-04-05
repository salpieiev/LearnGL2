//
//  Renderer9.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 4/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"
#include "ParametricSurface.h"
#include "Rotator.h"



class Renderer9 : public RenderingEngine
{
public:
    Renderer9(int width, int height);
    ~Renderer9();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void PrepareProgram();
    void GenerateBuffers();
    void SetupUniforms() const;
    void DrawSurface0() const;
    void DrawSurface1() const;
    
    Rotator *m_rotator;
    
    GLuint m_stencilProgram;
    
    ParametricSurface *m_surface0;
    ParametricSurface *m_surface1;
    
    GLuint m_surface0VertexBuffer;
    GLuint m_surface1VertexBuffer;
    GLuint m_surface0IndexBuffer;
    GLuint m_surface1IndexBuffer;
    GLuint m_surface0IndexCount;
    GLuint m_surface1IndexCount;
    
    GLint m_attribPosition;
    GLint m_attribColor;
    GLint m_attribNormal;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    GLuint m_uniformNormalMatrix;
    GLuint m_uniformLightPosition;
    GLuint m_uniformAmbientLight;
};
