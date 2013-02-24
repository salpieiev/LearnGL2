//
//  Renderer7.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 23.02.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer7.h"



Renderer7::Renderer7(int width, int height): RenderingEngine(width, height)
{
    
}

Renderer7::~Renderer7()
{
    
}

void Renderer7::Render() const
{
    glClearColor(0.5f, 0.7f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer7::OnFingerDown(ivec2 location)
{
    
}

void Renderer7::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer7::OnFingerUp(ivec2 location)
{
    
}