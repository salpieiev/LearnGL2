//
//  BoneChain.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/15/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "BoneChain.h"



BoneChain::BoneChain()
{
    
}

BoneChain::~BoneChain()
{
    
}

void BoneChain::SetStartPosition(vec3 startPosition)
{
    m_startPosition = startPosition;
}

vec3 BoneChain::GetStartPosition() const
{
    return m_startPosition;
}