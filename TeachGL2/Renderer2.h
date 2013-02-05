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
    void PrepareSurfaceProgram();
    void PrepareRoomProgram();
    void GenerateSurfaceBuffer();
    void GenerateRoomBuffer();
    void DrawSurface() const;
    void DrawRoom() const;
    
    Rotator *m_rotator;
    ParametricSurface *m_surface;
    
    // Surface
    GLuint m_program;
    
    GLuint m_surfaceVertexBuffer;
    GLuint m_surfaceIndexBuffer;
    GLuint m_surfaceIndexCount;
    
    GLint m_attribPosition;
    GLint m_attribSourceColor;
    GLint m_attribNormal;
    GLint m_attribLightDirection;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    GLuint m_uniformNormalMatrix;
    GLuint m_uniformAmbientLight;
    GLuint m_uniformSpecularLight;
    GLuint m_uniformShininess;
    
    // Room
    GLuint m_roomProgram;
    
    GLuint m_roomVertexBuffer;
    GLuint m_roomVertexCount;
    
    GLint m_attribRoomPosition;
    GLint m_attribRoomSourceColor;
    GLint m_attribRoomNormal;
    
    GLuint m_uniformRoomProjection;
    GLuint m_uniformRoomModelview;
    GLuint m_uniformRoomNormalMatrix;
    GLuint m_uniformRoomLightPosition;
    GLuint m_uniformRoomAmbientLight;
};
