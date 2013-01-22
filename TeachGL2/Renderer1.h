//
//  Renderer1.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"



class Renderer1: public RenderingEngine
{
public:
    Renderer1(int width, int height);
    ~Renderer1();
    
    void Render() const;
    void Update();
    
private:
};
