//
//  Bone.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/15/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Quaternion.h"



class Bone
{
public:
    Bone();
    Bone(vec3 projection);
    ~Bone();
    
    void SetProjection(vec3 projection);
    vec3 GetProjection() const;
    
private:
    vec3 m_projection;
//    vector
};