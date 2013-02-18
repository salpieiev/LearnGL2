//
//  Renderer6.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"
#include "ParametricSurface.h"
#include "Rotator.h"



class BoneChain;
struct Skeleton;
typedef vector<mat4> MatrixList;



class Renderer6: public RenderingEngine
{
public:
    Renderer6(int width, int height);
    ~Renderer6();
    
    void Render() const;
    
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void ComputeMatrices(const Skeleton &skeleton, MatrixList &matrices);
    
    Rotator *m_rotator;
    ParametricSurface *m_surface;
    BoneChain *m_chain;
    
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
    
    GLuint m_boneVertexBuffer;
    GLuint m_boneVertexCount;
    
//    GLuint m_vertexBuffer;
//    GLuint m_indexBuffer;
//    GLuint m_indexCount;
};
