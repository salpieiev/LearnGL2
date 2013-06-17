//
//  Renderer17.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 17.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



const int OffscreenCountGaussian = 5;

struct FramebuffersGaussian
{
    GLuint SceneFramebuffer;
    GLuint OffscreenFramebuffers[OffscreenCountGaussian];
};

struct RenderbuffersGaussian
{
    GLuint SceneColorRenderbuffer;
    GLuint SceneDepthRenderbuffer;
};

struct BloomTexturesGaussian
{
    GLuint BackgroundTexture;
    GLuint SceneTexture;
    GLuint OffscreenTextures[OffscreenCountGaussian];
};



class Renderer17 : public RenderingEngine
{
public:
    Renderer17(int width, int height);
    ~Renderer17();
    
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
    void GenerateBloomTexture() const;
    
    GLuint m_textureProgram;
    GLuint m_surfaceProgram;
    
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
    
    FramebuffersGaussian m_framebuffers;
    RenderbuffersGaussian m_renderbuffers;
    BloomTexturesGaussian m_textures;
};