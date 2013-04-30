//
//  Renderer14.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/30/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



class Renderer14 : public RenderingEngine
{
public:
    Renderer14(int width, int height);
    ~Renderer14();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void PrepareProgram();
    
    GLuint m_program;
};