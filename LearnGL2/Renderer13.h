//
//  Renderer13.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 26.04.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"
#include "Rotator.h"



class Renderer13: public RenderingEngine
{
public:
    Renderer13(int width, int height);
    ~Renderer13();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void PrepareProgram();
    void GenerateBuffers();
    void SetupUniforms() const;
    void DrawSphere() const;
    
    Rotator *m_rotator;
    
    GLuint m_program;
    
    GLint m_attribPosition;
    GLint m_attribColor;
    GLint m_attribNormal;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    GLuint m_uniformNormalMatrix;
    GLuint m_uniformLightPosition;
    GLuint m_uniformEyePosition;
    GLuint m_uniformAmbientColor;
    GLuint m_uniformSpecularColor;
    GLuint m_uniformShininess;
    
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_indexCount;
};
