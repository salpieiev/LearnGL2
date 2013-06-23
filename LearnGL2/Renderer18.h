//
//  Renderer18.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 22.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



class Renderer18 : public RenderingEngine
{
public:
    Renderer18(int width, int height);
    ~Renderer18();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void BuildLightingProgram();
    void GenerateSurfaceBuffers();
    void SetupLightingUniforms() const;
    void DrawSurface() const;
    
    GLuint m_lightingProgram;
    
    GLuint m_surfaceVertexBuffer;
    GLuint m_surfaceIndexBuffer;
    GLuint m_surfaceIndexCount;
    
    GLint m_attribLightingPosition;
    GLint m_attribLightingColor;
    
    GLuint m_uniformLightingProjection;
    GLuint m_uniformLightingModelview;
};