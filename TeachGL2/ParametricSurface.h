//
//  ParametricSurface.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/22/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "ISurface.h"
#include "Quaternion.h"



struct ParametricInterval
{
    ivec2 Divisions;
    vec2 UpperBound;
    vec2 TextureCount;
};



class ParametricSurface: public ISurface
{
public:
    int GetVertexCount() const;
    int GetLineIndexCount() const;
    int GetTriangleIndexCount() const;
    
    void GenerateVertices(vector<float> &vertices, unsigned char flags = 0, ivec3 boneIndices = ivec3(0, 0, 0)) const;
    void GenerateLineIndices(vector<unsigned short> &indices) const;
    void GenerateTriangleIndices(vector<unsigned short> &indices) const;
    
protected:
    void SetInterval(const ParametricInterval &interval);
    virtual vec3 Evaluate(const vec2 &domain) const = 0;
    virtual bool InvertNormal(const vec2 &domain) const;
    
private:
    vec2 ComputeDomain(float x, float y) const;
    
    ivec2 m_divisions;
    ivec2 m_slices;
    vec2 m_upperBound;
    vec2 m_textureCount;
};



class Cone: public ParametricSurface
{
public:
    Cone(float height, float radius);
    
    vec3 Evaluate(const vec2 &domain) const;
    
private:
    float m_height;
    float m_radius;
};


class Cylinder: public ParametricSurface
{
public:
    Cylinder(float height, float radius);
    
    vec3 Evaluate(const vec2 &domain) const;
    
private:
    float m_height;
    float m_radius;
};


class Sphere: public ParametricSurface
{
public:
    Sphere(float radius);
    
    vec3 Evaluate(const vec2 &domain) const;
    
private:
    float m_radius;
};


class Torus: public ParametricSurface
{
public:
    Torus(float majorRadius, float minorRadius);
    
    vec3 Evaluate(const vec2 &domain) const;
    
private:
    float m_majorRadius;
    float m_minorRadius;
};


class TrefoilKnot: public ParametricSurface
{
public:
    TrefoilKnot(float scale);
    
    vec3 Evaluate(const vec2 &domain) const;
    
private:
    float m_scale;
};


class MobiusStrip: public ParametricSurface
{
public:
    MobiusStrip(float scale);
    
    vec3 Evaluate(const vec2 &domain) const;
    
private:
    float m_scale;
};


class KleinBottle: public ParametricSurface
{
public:
    KleinBottle(float scale);
    
    vec3 Evaluate(const vec2 &domain) const;
    bool InvertNormal(const vec2 &domain) const;
    
private:
    float m_scale;
};


