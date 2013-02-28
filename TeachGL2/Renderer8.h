//
//  Renderer8.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/28/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"



class Renderer8 : public RenderingEngine
{
public:
    Renderer8(int width, int height);
    ~Renderer8();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void BuildFogProgram();
    void GenerateCubeData();
    void DrawCube() const;
    void ApplyFrustum() const;
    
    GLuint m_fogProgram;
    
    GLuint m_cubeVertexBuffer;
    GLuint m_cubeIndexBuffer;
    GLuint m_cubeIndexCount;
    
    GLint m_attribPosition;
    GLint m_attribColor;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
};
