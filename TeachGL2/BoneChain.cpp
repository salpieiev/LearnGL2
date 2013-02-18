//
//  BoneChain.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/15/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "BoneChain.h"
#include <iostream>

using namespace std;



BoneChain::BoneChain(): m_startPosition(0.0f, 0.0f, 0.0f)
{
    m_bones = new vector<Bone>;
}

BoneChain::~BoneChain()
{
    delete m_bones;
}

void BoneChain::SetStartPosition(vec3 startPosition)
{
    m_startPosition = startPosition;
}

vec3 BoneChain::GetStartPosition() const
{
    return m_startPosition;
}

void BoneChain::AddBone(const Bone &bone) const
{
    m_bones->push_back(bone);
}

vector<Bone> * BoneChain::GetBones() const
{
    return m_bones;
}

vector<float> BoneChain::GetVertexData() const
{
    int vertexCount = m_bones->size() + 1;
    vector<float> vertexData(vertexCount * 3);
    
    float *attribute = &vertexData[0];
    attribute = vec3(m_startPosition).Write(attribute);
    
    vec3 curPosition = m_startPosition;
    
    vector<Bone>::const_iterator bone;
    for (bone = m_bones->begin(); bone != m_bones->end(); bone++)
    {
        curPosition += bone->GetProjection();
        attribute = curPosition.Write(attribute);
    }
    
    return vertexData;
}

unsigned int BoneChain::GetVertexCount() const
{
    return m_bones->size() + 1;
}