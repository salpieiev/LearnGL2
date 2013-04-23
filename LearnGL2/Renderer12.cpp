//
//  Renderer12.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/23/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer12.h"



Renderer12::Renderer12(int width, int height): RenderingEngine(width, height)
{
//    glViewport(0, 0, width, height);
}

Renderer12::~Renderer12()
{
    
}

void Renderer12::Render() const
{
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer12::OnFingerDown(ivec2 location)
{
    
}

void Renderer12::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer12::OnFingerUp(ivec2 location)
{
    
}