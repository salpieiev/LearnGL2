//
//  Renderer15.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 20.05.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"
#include "Rotator.h"



class Renderer15 : public RenderingEngine
{
public:
    Renderer15(int width, int height);
    ~Renderer15();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void BuildSurfaceProgram();
    void BuildTextureProgram();
    void GenerateSurfaceBuffers();
    void DrawSurface() const;
    void DrawBlurredTexture() const;
    void GenerateTexture();
    
    Rotator *m_rotator;
    
    GLuint m_program;
    GLuint m_textureProgram;
    GLuint m_texture;
    
    GLint m_attribPosition;
    GLint m_attribSourceColor;
    GLint m_attribNormal;
    GLint m_attribTexPosition;
    GLint m_attribTexCoord;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    GLuint m_uniformNormalMatrix;
    GLuint m_uniformLightPosition;
    GLuint m_uniformAmbientLight;
    GLuint m_uniformSpecularLight;
    GLuint m_uniformShininess;
    GLuint m_uniformTexProjection;
    GLuint m_uniformTexModelview;
    GLuint m_uniformTexBlurStep;
    
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_indexCount;
};
