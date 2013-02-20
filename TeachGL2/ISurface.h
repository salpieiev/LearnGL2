//
//  ISurface.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/22/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include <vector>
#include "Vector.h"

using std::vector;



enum VertexFlags
{
    VertexFlagsColors = 1 << 0,
    VertexFlagsNormals = 1 << 1,
    VertexFlagsLightDirection = 1 << 2,
    VertexFlagsTexCoords = 1 << 3,
    VertexFlagsBoneWeights = 1 << 4
};



class ISurface
{
public:
    virtual ~ISurface() {}
    
    virtual int GetVertexCount() const = 0;
    virtual int GetLineIndexCount() const = 0;
    virtual int GetTriangleIndexCount() const = 0;
    
    virtual void GenerateVertices(vector<float> &vertices, unsigned char flags = 0, ivec2 boneIndices = ivec2(0, 0)) const = 0;
    virtual void GenerateLineIndices(vector<unsigned short> &indices) const = 0;
    virtual void GenerateTriangleIndices(vector<unsigned short> &indices) const = 0;
};
