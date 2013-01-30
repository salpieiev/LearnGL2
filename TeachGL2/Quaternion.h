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
    
    inline QuaternionT<T> Scaled(T scale) const;
    inline QuaternionT<T> Rotated(const QuaternionT<T> &b) const;
    inline T InnerProduct(const QuaternionT<T> &q) const;
    inline T Norm() const;
    inline T Magnitude() const;
    inline Matrix3<T> ToMatrix() const;
    inline Vector4<T> ToVector() const;
    inline void Normalize();
    
    static inline QuaternionT<T> CreateFromVectors(const Vector3<T> &v0, const Vector3<T> &v1);
    static inline QuaternionT<T> CreateFromAxisAngle(const Vector3<T> &axis, float radians);
    
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
inline QuaternionT<T> QuaternionT<T>::Scaled(T scale) const
{
    QuaternionT<T> scaled(x * scale, y * scale, z * scale, w * scale);
    return scaled;
}

template <typename T>
inline QuaternionT<T> QuaternionT<T>::Rotated(const QuaternionT<T> &b) const
{
    QuaternionT<T> q;
    q.w = w * b.w - x * b.x - y * b.y - z * b.z;
    q.x = w * b.x + x * b.w + y * b.z - z * b.y;
    q.y = w * b.y + y * b.w + z * b.x - x * b.z;
    q.z = w * b.z + z * b.w + x * b.y - y * b.x;
    q.Normalize();
    return q;
}

template <typename T>
inline T QuaternionT<T>::InnerProduct(const QuaternionT<T> &q) const
{
    T dot = x * q.x + y * q.y + z * q.z + w * q.w;
    return dot;
}

template <typename T>
inline T QuaternionT<T>::Norm() const
{
    T norm = InnerProduct(*this);
    return norm;
}

template <typename T>
inline T QuaternionT<T>::Magnitude() const
{
    T magnitude = sqrt(Norm());
    return magnitude;
}

template <typename T>
inline Matrix3<T> QuaternionT<T>::ToMatrix() const
{
    Matrix3<T> m;
    m.x.x = 1.0f - 2.0f * (y * y + z * z);
    m.x.y = 2.0f * (x * y - z * w);
    m.x.z = 2.0f * (y * w + x * z);
    m.y.x = 2.0f * (x * y + w * z);
    m.y.y = 1.0f - 2.0f * (x * x + z * z);
    m.y.z = 2.0f * (y * z - x * w);
    m.z.x = 2.0f * (x * z - y * w);
    m.z.y = 2.0f * (y * z + x * w);
    m.z.z = 1.0f - 2.0f * (x * x + y * y);
    return m;
}

template <typename T>
inline Vector4<T> QuaternionT<T>::ToVector() const
{
    Vector4<T> vector = Vector4<T>(x, y, z, w);
    return vector;
}

template <typename T>
inline void QuaternionT<T>::Normalize()
{
    T scale = 1.0f / Magnitude();
    *this = Scaled(scale);
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