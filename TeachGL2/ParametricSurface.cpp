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
    if (flags & VertexFlagsColors)
        floatsPerVertex += 4;
    if (flags & VertexFlagsNormals)
        floatsPerVertex += 3;
    if (flags & VertexFlagsLightDirection)
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
            
            // Set color
            if (flags & VertexFlagsColors)
            {
                vec4 color = vec4(1.0f, 1.0f, 0.5f, 1.0f);
                attribute = color.Write(attribute);
            }
            
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
            
            // Compute light position
            if (flags & VertexFlagsLightDirection)
            {
                vec3 lightPosition(0.0f, 0.0f, 0.0f);
                vec3 lightDirection = lightPosition - range;
                lightDirection.Normalize();
                
                attribute = lightDirection.Write(attribute);
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
    indices.resize(GetLineIndexCount());
    vector<unsigned short>::iterator index = indices.begin();
    
    for (int j = 0, vertex = 0; j < m_slices.y; j++)
    {
        for (int i = 0; i < m_slices.x; i++)
        {
            int next = (i + 1) % m_divisions.x;
            *index++ = vertex + i;
            *index++ = vertex + next;
            *index++ = vertex + i;
            *index++ = vertex + i + m_divisions.x;
        }
        
        vertex += m_divisions.x;
    }
}

void ParametricSurface::GenerateTriangleIndices(vector<unsigned short> &indices) const
{
    indices.resize(GetTriangleIndexCount());
    vector<unsigned short>::iterator index = indices.begin();
    
    for (int j = 0, vertex = 0; j < m_slices.y; j++)
    {
        for (int i = 0; i < m_slices.x; i++)
        {
            int next = (i + 1) % m_divisions.x;
            *index++ = vertex + i;
            *index++ = vertex + next;
            *index++ = vertex + i + m_divisions.x;
            *index++ = vertex + next;
            *index++ = vertex + next + m_divisions.x;
            *index++ = vertex + i + m_divisions.x;
        }
        
        vertex += m_divisions.x;
    }
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
    ParametricInterval parametricInterval = { ivec2(80, 80), vec2(TwoPi, 1.0f), vec2(30.0f, 20.0f)};
    SetInterval(parametricInterval);
}

vec3 Cone::Evaluate(const vec2 &domain) const
{
    float u = domain.x;
    float v = domain.y;
    
    float x = m_radius * (1 - v) * cos(u);
    float y = m_height * (v - 0.5f);
    float z = m_radius * (1 - v) * -sin(u);
    
    vec3 range = vec3(x, y, z);
    return range;
}


Sphere::Sphere(float radius): m_radius(radius)
{
    ParametricInterval parametricInterval = { ivec2(160, 160), vec2(Pi, TwoPi), vec2(20, 35) };
    SetInterval(parametricInterval);
}

vec3 Sphere::Evaluate(const vec2 &domain) const
{
    float u = domain.x;
    float v = domain.y;
    
    float x = m_radius * sin(u) * cos(v);
    float y = m_radius * cos(u);
    float z = m_radius * -sin(u) * sin(v);
    
    vec3 range = vec3(x, y, z);
    return range;
}


Torus::Torus(float majorRadius, float minorRadius): m_majorRadius(majorRadius), m_minorRadius(minorRadius)
{
    ParametricInterval parametricInterval = { ivec2(80, 80), vec2(TwoPi, TwoPi), vec2(40, 10) };
    SetInterval(parametricInterval);
}

vec3 Torus::Evaluate(const vec2 &domain) const
{
    float u = domain.x;
    float v = domain.y;
    
    float x = (m_majorRadius + m_minorRadius * cos(v)) * cos(u);
    float y = (m_majorRadius + m_minorRadius * cos(v)) * sin(u);
    float z = m_minorRadius * sin(v);
    
    vec3 range = vec3(x, y, z);
    return range;
}


TrefoilKnot::TrefoilKnot(float scale): m_scale(scale)
{
    ParametricInterval parametricInterval = { ivec2(20, 10), vec2(TwoPi, TwoPi), vec2(100, 8) };
    SetInterval(parametricInterval);
}

vec3 TrefoilKnot::Evaluate(const vec2 &domain) const
{
    const float a = 0.5f;
    const float b = 0.3f;
    const float c = 0.5f;
    const float d = 0.1f;
    
    float u = (TwoPi - domain.x) * 2;
    float v = domain.y;
    
    float r = a + b * cos(1.5f * u);
    float x = r * cos(u);
    float y = r * sin(u);
    float z = c * sin(1.5f * u);
    
    vec3 dv;
    dv.x = -1.5f * b * sin(1.5f * u) * cos(u) - (a + b * cos(1.5f * u)) * sin(u);
    dv.y = -1.5f * b * sin(1.5f * u) * sin(u) + (a + b * cos(1.5f * u)) * cos(u);
    dv.z = 1.5f * c * cos(1.5f * u);
    
    vec3 q = dv.Normalized();
    vec3 qvn = vec3(q.y, -q.x, 0).Normalized();
    vec3 ww = q.Cross(qvn);
    
    vec3 range;
    range.x = x + d * (qvn.x * cos(v) + ww.x * sin(v));
    range.y = y + d * (qvn.y * cos(v) + ww.y * sin(v));
    range.z = z + d * ww.z * sin(v);
    range = range * m_scale;
    
    return range;
}


MobiusStrip::MobiusStrip(float scale): m_scale(scale)
{
    ParametricInterval parametricInterval = { ivec2(100, 60), vec2(TwoPi, TwoPi), vec2(40, 15) };
    SetInterval(parametricInterval);
}

vec3 MobiusStrip::Evaluate(const vec2 &domain) const
{
    float u = domain.x;
    float t = domain.y;
    
    float major = 1.25;
    float a = 0.125f;
    float b = 0.5f;
    float phi = u / 2;
    
    // General equation for an ellipse where phi is the angle
    // between the major axis and the X axis.
    float x = a * cos(t) * cos(phi) - b * sin(t) * sin(phi);
    float y = a * cos(t) * sin(phi) + b * sin(t) * cos(phi);
    
    // Sweep the ellipse along a circle, like a torus.
    vec3 range;
    range.x = (major + x) * cos(u);
    range.y = (major + x) * sin(u);
    range.z = y;
    range = range * m_scale;
    
    return range;
}


KleinBottle::KleinBottle(float scale): m_scale(scale)
{
    ParametricInterval parametricInterval = { ivec2(80, 80), vec2(TwoPi, TwoPi), vec2(15, 50) };
    SetInterval(parametricInterval);
}

vec3 KleinBottle::Evaluate(const vec2 &domain) const
{
    float v = 1 - domain.x;
    float u = domain.y;
    
    float x0 = 3 * cos(u) * (1 + sin(u)) + (2 * (1 - cos(u) / 2)) * cos(u) * cos(v);
    float y0 = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);
    float x1 = 3 * cos(u) * (1 + sin(u)) + (2 * (1 - cos(u) / 2)) * cos(v + Pi);
    float y1 = 8 * sin(u);
    
    vec3 range;
    range.x = u < Pi ? x0 : x1;
    range.y = u < Pi ? -y0 : -y1;
    range.z = (-2 * (1 - cos(u) / 2)) * sin(v);
    range = range * m_scale;
    
    return range;
}

bool KleinBottle::InvertNormal(const vec2 &domain) const
{
    bool invertNormal = domain.y > 3 * Pi / 2;
    return invertNormal;
}




