//
//  BoneChain.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/15/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Bone.h"
#include <vector>

using namespace std;



class BoneChain
{
public:
    BoneChain();
    ~BoneChain();
    
    void SetStartPosition(vec3 startPosition);
    vec3 GetStartPosition() const;
    
    void AddBone(const Bone &bone) const;
    
    vector<float> GetVertexData() const;
    unsigned int GetVertexCount() const;
    
private:
    vec3 m_startPosition;
    vector<Bone> *m_bones;
};