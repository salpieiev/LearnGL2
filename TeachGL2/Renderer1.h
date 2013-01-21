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
    Renderer1();
    ~Renderer1();
    
    void Render(int width, int height) const;
    void Update();
    
private:
};
