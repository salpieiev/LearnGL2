//
//  Renderer9.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 4/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "RenderingEngine.h"



class Renderer9 : public RenderingEngine
{
public:
    Renderer9(int width, int height);
    ~Renderer9();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    
};
