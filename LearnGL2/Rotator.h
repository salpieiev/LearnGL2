//
//  Rotator.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 2/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "Quaternion.h"



class Rotator
{
public:
    Rotator(ivec2 areaSize);
    ~Rotator();
    
    void Start(ivec2 location);
    void Move(ivec2 location);
    void End(ivec2 location);
    
    Quaternion GetOrientation() const;
    
private:
    vec3 MapToSphere(ivec2 touchLocation) const;
    
    float m_trackballRadius;
    ivec2 m_centerPoint;
    ivec2 m_fingerStart;
    Quaternion m_orientation;
    Quaternion m_prevOrientation;
};
