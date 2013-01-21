//
//  Renderer1.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer1.h"



struct Vertex
{
    vec3 Position;
    vec4 Color;
};



Renderer1::Renderer1()
{
    
}

Renderer1::~Renderer1()
{
    
}

void Renderer1::Render(int width, int height) const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer1::Update()
{
    
}