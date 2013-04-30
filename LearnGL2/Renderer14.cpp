//
//  Renderer14.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/30/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer14.h"
#include "Shaders/NormalMapGenerator.vsh"
#include "Shaders/NormalMapGenerator.fsh"



Renderer14::Renderer14(int width, int height): RenderingEngine(width, height)
{
    
}

Renderer14::~Renderer14()
{
    
}

void Renderer14::Render() const
{
    
}

void Renderer14::OnFingerDown(ivec2 location)
{
    
}

void Renderer14::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer14::OnFingerUp(ivec2 location)
{
    
}

void Renderer14::PrepareProgram()
{
    m_program = BuildProgram(NormalMapGeneratorVertexShader, NormalMapGeneratorFragmentShader);
    glUseProgram(m_program);
    
    
}