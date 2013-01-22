//
//  ParametricSurface.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/22/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "ParametricSurface.h"



int ParametricSurface::GetVertexCount() const
{
    int vertexCount = m_divisions.x * m_divisions.y;
    return vertexCount;
}

int ParametricSurface::GetLineIndexCount() const
{
    int lineIndexCount = 4 * m_slices.x * m_slices.y;
    return lineIndexCount;
}

int ParametricSurface::GetTriangleIndexCount() const
{
    int triangleIndexCount = 6 * m_slices.x * m_slices.y;
    return triangleIndexCount;
}

void ParametricSurface::GenerateVertices(vector<float> &vertices, unsigned char flags) const
{
    int floatsPerVertex = 3;
    if (flags & VertexFlagsNormals)
        floatsPerVertex += 3;
    if (flags & VertexFlagsTexCoords)
        floatsPerVertex += 2;
    
    vertices.resize(GetVertexCount() * floatsPerVertex);
    float *attribute = &vertices[0];
    
    for (int j = 0; j < m_divisions.y; j++)
    {
        for (int i = 0; i < m_divisions.x; i++)
        {
            // Compute position
            vec2 domain = ComputeDomain(i, j);
            vec3 range = Evaluate(domain);
            attribute = range.Write(attribute);
            
            // Compute normal
            if (flags & VertexFlagsNormals)
            {
                float s = (float)i;
                float t = (float)j;
                
                // Nudge the point if the normal is indeterminate
                if (i == 0) s += 0.01f;
                if (i == m_divisions.x - 1) s -= 0.01f;
                if (j == 0) t += 0.01f;
                if (j == m_divisions.y - 1) t -= 0.01f;
                
                // Compute the tangents and their cross product
                vec3 p = Evaluate(ComputeDomain(s, t));
                vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
                vec3 v = Evaluate(ComputeDomain(s, t + 0.01f)) - p;
                vec3 normal = u.Cross(v).Normalized();
                
                if (InvertNormal(domain))
                {
                    normal = -normal;
                }
                
                attribute = normal.Write(attribute);
            }
            
            // Compute texture coordinates
            if (flags & VertexFlagsTexCoords)
            {
                float s = m_textureCount.x * i / m_slices.x;
                float t = m_textureCount.y * j / m_slices.y;
                vec2 texCoord = vec2(s, t);
                attribute = texCoord.Write(attribute);
            }
        }
    }
}

void ParametricSurface::GenerateLineIndices(vector<unsigned short> &indices) const
{
    
}

void ParametricSurface::GenerateTriangleIndices(vector<unsigned short> &indices) const
{
    
}

void ParametricSurface::SetInterval(const ParametricInterval &interval)
{
    m_divisions = interval.Divisions;
    m_upperBound = interval.UpperBound;
    m_textureCount = interval.TextureCount;
    m_slices = m_divisions - ivec2(1, 1);
}

bool ParametricSurface::InvertNormal(const vec2 &domain) const
{
    return false;
}

vec2 ParametricSurface::ComputeDomain(float x, float y) const
{
    float domainX = x * m_upperBound.x / m_slices.x;
    float domainY = y * m_upperBound.y / m_slices.y;
    vec2 domain = vec2(domainX, domainY);
    return domain;
}



Cone::Cone(float height, float radius): m_height(height), m_radius(radius)
{
    
}

vec3 Cone::Evaluate(const vec2 &domain) const
{
    return vec3();
}


vec3 Sphere::Evaluate(const vec2 &domain) const
{
    return vec3();
}


vec3 Torus::Evaluate(const vec2 &domain) const
{
    return vec3();
}


vec3 TrefoilKnot::Evaluate(const vec2 &domain) const
{
    return vec3();
}


vec3 MobiusStrip::Evaluate(const vec2 &domain) const
{
    return vec3();
}


vec3 KleinBottle::Evaluate(const vec2 &domain) const
{
    return vec3();
}




