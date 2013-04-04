//
//  Renderer9.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 4/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer9.h"



Renderer9::Renderer9(int width, int height): RenderingEngine(width, height)
{
    
}

Renderer9::~Renderer9()
{
    
}

void Renderer9::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer9::OnFingerDown(ivec2 location)
{
    
}

void Renderer9::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer9::OnFingerUp(ivec2 location)
{
    
}
