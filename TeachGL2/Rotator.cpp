//
//  Rotator.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Rotator.h"



Rotator::Rotator(ivec2 areaSize)
{
    m_trackballRadius = (float)areaSize.x / 3.0f;
    m_centerPoint = areaSize / 2;
}

Rotator::~Rotator()
{
    
}

void Rotator::Start(ivec2 location)
{
    m_fingerStart = location;
    m_prevOrientation = m_orientation;
}

void Rotator::Move(ivec2 location)
{
    vec3 start = MapToSphere(m_fingerStart);
    vec3 end = MapToSphere(location);
    
    Quaternion delta = Quaternion::CreateFromVectors(start, end);
    m_orientation = delta.Rotated(m_prevOrientation);
}

void Rotator::End(ivec2 location)
{
    
}

Quaternion Rotator::GetOrientation() const
{
    return m_orientation;
}

vec3 Rotator::MapToSphere(ivec2 touchLocation) const
{
    vec2 p = touchLocation - m_centerPoint;
    
    // Flip the y-axis because pixel coords increase toward the bottom
    p.y = -p.y;
    
    const float safeRadius = m_trackballRadius - 1.0f;
    
    if (p.Length() > safeRadius)
    {
        float theta = atan2(p.y, p.x);
        p.x = safeRadius * cos(theta);
        p.y = safeRadius * sin(theta);
    }
    
    float z = sqrtf(m_trackballRadius * m_trackballRadius - p.LengthSquared());
    vec3 mapped = vec3(p.x, p.y, z);
    mapped /= m_trackballRadius;
    
    return mapped;
}