//
//  Quaternion.h
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#pragma once
#include "Matrix.h"



template <typename T>
class QuaternionT
{
public:
    QuaternionT();
    QuaternionT(T x, T y, T z, T w);
    
    static QuaternionT<T> CreateFromVectors(const Vector3<T> &v0, const Vector3<T> &v1);
    static QuaternionT<T> CreateFromAxisAngle(const Vector3<T> &axis, float radians);
    
    T x;
    T y;
    T z;
    T w;
};



template <typename T>
inline QuaternionT<T>::QuaternionT(): x(0), y(0), z(0), w(1)
{
    
}

template <typename T>
inline QuaternionT<T>::QuaternionT(T x, T y, T z, T w): x(x), y(y), z(z), w(w)
{
    
}

template <typename T>
inline QuaternionT<T> QuaternionT<T>::CreateFromVectors(const Vector3<T> &v0, const Vector3<T> &v1)
{
    if (v0 == -v1)
    {
        return QuaternionT<T>::CreateFromAxisAngle(vec3(1.0f, 0.0f, 0.0f), Pi);
    }
    
    Vector3<T> cross = v0.Cross(v1);
    T dot = v0.Dot(v1);
    T s = sqrt((1 + dot) * 2);
    
    QuaternionT<T> q;
    q.x = cross.x / s;
    q.y = cross.y / s;
    q.z = cross.z / s;
    q.w = s / 2.0f;
    
    return q;
}

template <typename T>
inline QuaternionT<T> QuaternionT<T>::CreateFromAxisAngle(const Vector3<T> &axis, float radians)
{
    QuaternionT<T> q;
    q.w = cos(radians / 2.0f);
    q.x = q.y = q.z = sin(radians / 2.0f);
    q.x *= axis.x;
    q.y *= axis.y;
    q.z *= axis.z;
    return q;
}



typedef QuaternionT<float> Quaternion;