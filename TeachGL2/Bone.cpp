//
//  Bone.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/15/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Bone.h"



Bone::Bone()
{
    
}

Bone::Bone(vec3 projection): m_projection(projection)
{
    
}

Bone::~Bone()
{
    
}

void Bone::SetProjection(vec3 projection)
{
    m_projection = projection;
}

vec3 Bone::GetProjection() const
{
    return m_projection;
}