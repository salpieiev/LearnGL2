//
//  Renderer12.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/23/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"
#include "ParametricSurface.h"
#include "Rotator.h"



class Renderer12 : public RenderingEngine
{
public:
    Renderer12(int width, int height);
    ~Renderer12();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void PrepareProgram();
    void GenerateBuffers();
    void LoadTexture();
    void DrawSphere() const;
    
    Rotator *m_rotator;
    
    GLuint m_program;
    GLuint m_texture;
    
    GLint m_attribPosition;
    GLint m_attribTexCoord;
    
    GLuint m_uniformProjection;
    GLuint m_uniformModelview;
    
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_indexCount;
};
