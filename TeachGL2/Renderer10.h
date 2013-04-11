//
//  Renderer10.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 4/9/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



class Renderer10: public RenderingEngine
{
public:
    Renderer10(int width, int height);
    ~Renderer10();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void DrawQuad() const;
    
    GLuint m_program;
    
    GLint m_attribPosition;
    GLint m_attribColor;
    
    GLuint m_uniformModelview;
};
