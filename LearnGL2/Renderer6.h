//
//  Renderer6.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"
#include "ParametricSurface.h"
#include "Rotator.h"
#include "BoneChain.h"



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
    void PrepareBoneProgram();
    void PrepareSkinProgram();
    void GenerateBoneData();
    void GenerateSkinData();
    void SetupBoneUniforms();
    void SetupSkinUniforms();
    void DrawBones() const;
    void DrawSkin() const;
    void ComputeMatrices(vector<mat4> &matrices);
    
    Rotator *m_rotator;
    ParametricSurface *m_skinCylinder;
    vector<float> m_skinVertices;
    vector<GLushort> m_skinIndices;
    BoneChain *m_chain;
    vector<mat4> m_matrices;
    
    GLuint m_boneProgram;
    GLuint m_skinProgram;
    
    GLint m_attribBonePosition;
    GLint m_attribBoneSourceColor;
    GLint m_attribSkinPosition;
    GLint m_attribSkinSourceColor;
    GLint m_attribSkinBoneWeights;
    GLint m_attribSkinBoneIndices;
    
    GLuint m_uniformBoneProjection;
    GLuint m_uniformBoneModelview;
    GLuint m_uniformSkinProjection;
    GLuint m_uniformSkinModelview;
    
    GLuint m_boneVertexBuffer;
    GLuint m_skinVertexBuffer;
    GLuint m_skinIndexBuffer;
};
