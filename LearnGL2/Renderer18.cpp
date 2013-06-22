//
//  Renderer18.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 22.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer18.h"



Renderer18::Renderer18(int width, int height): RenderingEngine(width, height)
{
    
}

Renderer18::~Renderer18()
{
    
}

void Renderer18::Render() const
{
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer18::OnFingerDown(ivec2 location)
{
    
}

void Renderer18::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer18::OnFingerUp(ivec2 location)
{
    
}