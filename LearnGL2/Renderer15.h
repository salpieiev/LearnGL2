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
    void DrawSurface() const;
    void GenerateTexture();
    
    Rotator *m_rotator;
    
    GLuint m_program;
    GLuint m_texture;
    
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
