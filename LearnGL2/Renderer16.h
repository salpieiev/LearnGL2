//
//  Renderer16.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 05.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"
#include "Rotator.h"



class Renderer16 : public RenderingEngine
{
public:
    Renderer16(int width, int height);
    ~Renderer16();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void BuildTextureProgram();
    void BuildSurfaceProgram();
    void LoadTexture();
    void GenerateSurfaceBuffers();
    void DrawTexture() const;
    void DrawSurface() const;
    
    Rotator *m_rotator;
    
    GLuint m_textureProgram;
    GLuint m_surfaceProgram;
    
    GLuint m_backgroundTexture;
    
    GLint m_attribTexturePosition;
    GLint m_attribTextureCoord;
    GLint m_attribSurfacePosition;
    GLint m_attribSurfaceSourceColor;
    GLint m_attribSurfaceNormal;
    
    GLuint m_uniformTextureThreshold;
    GLuint m_uniformSurfaceProjection;
    GLuint m_uniformSurfaceModelview;
    GLuint m_uniformSurfaceNormalMatrix;
    GLuint m_uniformSurfaceLightPosition;
    GLuint m_uniformSurfaceAmbientLight;
    GLuint m_uniformSurfaceSpecularLight;
    GLuint m_uniformSurfaceShininess;
    
    GLuint m_surfaceVertexBuffer;
    GLuint m_surfaceIndexBuffer;
    GLuint m_surfaceIndexCount;
};
