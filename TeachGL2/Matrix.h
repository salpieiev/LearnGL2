//
//  Matrix.h
//  TeachGL
//
//  Created by Sergey Alpeev on 29.12.12.
//  Copyright (c) 2012 Brisk Mobile Inc. All rights reserved.
//

#pragma once
#include "Vector.h"



template <typename T>
class Matrix3
{
public:
    Matrix3();
    
    const T * Pointer() const;
    
    Vector3<T> operator * (const Vector3<T> v) const;
    
    Matrix3<T> Transposed() const;
    
    vec3 x;
    vec3 y;
    vec3 z;
};



template <typename T>
class Matrix4
{
public:
    Matrix4();
    Matrix4(const Matrix3<T> &m);
    
    const T * Pointer() const;
    
    Matrix4<T> operator * (const Matrix4<T> m) const;
    Matrix3<T> ToMat3() const;
    
    static inline Matrix4<T> Translate(T x, T y, T z);
    static inline Matrix4<T> Scale(T x, T y, T z);
    static inline Matrix4<T> RotateX(T degrees);
    static inline Matrix4<T> RotateY(T degrees);
    static inline Matrix4<T> RotateZ(T degrees);
    static inline Matrix4<T> Frustum(T left, T right, T bottom, T top, T near, T far);
    
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
};




template <typename T>
Matrix3<T>::Matrix3()
{
    
}

template <typename T>
const T * Matrix3<T>::Pointer() const
{
    return &x.x;
}

template <typename T>
Vector3<T> Matrix3<T>::operator * (const Vector3<T> v) const
{
    Vector3<T> vector;
    vector.x = x.x * v.x + x.y * v.y + x.z * v.z;
    vector.y = y.x * v.x + y.y * v.y + y.z * v.z;
    vector.z = z.x * v.x + z.y * v.y + z.z * v.z;
    return vector;
}

template <typename T>
Matrix3<T> Matrix3<T>::Transposed() const
{
    Matrix3<T> m;
    m.x.x = x.x; m.x.y = y.x; m.x.z = z.x;
    m.y.x = x.y; m.y.y = y.y; m.y.z = z.y;
    m.z.x = x.z; m.z.y = y.z; m.z.z = z.z;
    return m;
}



template <typename T>
Matrix4<T>::Matrix4()
{
    x.x = 1.0f; x.y = 0.0f; x.z = 0.0f; x.w = 0.0f;
    y.x = 0.0f; y.y = 1.0f; y.z = 0.0f; y.w = 0.0f;
    z.x = 0.0f; z.y = 0.0f; z.z = 1.0f; z.w = 0.0f;
    w.x = 0.0f; w.y = 0.0f; w.z = 0.0f; w.w = 1.0f;
}

template <typename T>
Matrix4<T>::Matrix4(const Matrix3<T> &m)
{
    x.x = m.x.x; x.y = m.x.y; x.z = m.x.z; x.w = 0.0f;
    y.x = m.y.x; y.y = m.y.y; y.z = m.y.z; y.w = 0.0f;
    z.x = m.z.x; z.y = m.z.y; z.z = m.z.z; z.w = 0.0f;
    w.x = 0.0f;  w.y = 0.0f;  w.z = 0.0f;  w.w = 1.0f;
}

template <typename T>
inline const T * Matrix4<T>::Pointer() const
{
    return &x.x;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::operator * (const Matrix4<T> b) const
{
    Matrix4<T> m;
    
    m.x.x = x.x * b.x.x + x.y * b.y.x + x.z * b.z.x + x.w * b.w.x;
    m.x.y = x.x * b.x.y + x.y * b.y.y + x.z * b.z.y + x.w * b.w.y;
    m.x.z = x.x * b.x.z + x.y * b.y.z + x.z * b.z.z + x.w * b.w.z;
    m.x.w = x.x * b.x.w + x.y * b.y.w + x.z * b.z.w + x.w * b.w.w;
    
    m.y.x = y.x * b.x.x + y.y * b.y.x + y.z * b.z.x + y.w * b.w.x;
    m.y.y = y.x * b.x.y + y.y * b.y.y + y.z * b.z.y + y.w * b.w.y;
    m.y.z = y.x * b.x.z + y.y * b.y.z + y.z * b.z.z + y.w * b.w.z;
    m.y.w = y.x * b.x.w + y.y * b.y.w + y.z * b.z.w + y.w * b.w.w;
    
    m.z.x = z.x * b.x.x + z.y * b.y.x + z.z * b.z.x + z.w * b.w.x;
    m.z.y = z.x * b.x.y + z.y * b.y.y + z.z * b.z.y + z.w * b.w.y;
    m.z.z = z.x * b.x.z + z.y * b.y.z + z.z * b.z.z + z.w * b.w.z;
    m.z.w = z.x * b.x.w + z.y * b.y.w + z.z * b.z.w + z.w * b.w.w;
    
    m.w.x = w.x * b.x.x + w.y * b.y.x + w.z * b.z.x + w.w * b.w.x;
    m.w.y = w.x * b.x.y + w.y * b.y.y + w.z * b.z.y + w.w * b.w.y;
    m.w.z = w.x * b.x.z + w.y * b.y.z + w.z * b.z.z + w.w * b.w.z;
    m.w.w = w.x * b.x.w + w.y * b.y.w + w.z * b.z.w + w.w * b.w.w;
    
    return m;
}

template <typename T>
inline Matrix3<T> Matrix4<T>::ToMat3() const
{
    Matrix3<T> m;
    m.x.x = x.x; m.x.y = x.y; m.x.z = x.z;
    m.y.x = y.x; m.y.y = y.y; m.y.z = y.z;
    m.z.x = z.x; m.z.y = z.y; m.z.z = z.z;
    return m;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::Translate(T x, T y, T z)
{
    Matrix4 m;
    m.x.x = 1.0f; m.x.y = 0.0f; m.x.z = 0.0f; m.x.w = 0.0f;
    m.y.x = 0.0f; m.y.y = 1.0f; m.y.z = 0.0f; m.y.w = 0.0f;
    m.z.x = 0.0f; m.z.y = 0.0f; m.z.z = 1.0f; m.z.w = 0.0f;
    m.w.x = x;    m.w.y = y;    m.w.z = z;    m.w.w = 1.0f;
    return m;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::Scale(T x, T y, T z)
{
    Matrix4<T> m;
    m.x.x = x;    m.x.y = 0.0f; m.x.z = 0.0f; m.x.w = 0.0f;
    m.y.x = 0.0f; m.y.y = y;    m.y.z = 0.0f; m.y.w = 0.0f;
    m.z.x = 0.0f; m.z.y = 0.0f; m.z.z = z;    m.z.w = 0.0f;
    m.w.x = 0.0f; m.w.y = 0.0f; m.w.z = 0.0f; m.w.w = 1.0f;
    return m;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::RotateX(T degrees)
{
    T radians = degrees * Pi / 180.0f;
    T s = std::sin(radians);
    T c = std::cos(radians);
    
    Matrix4<T> m;
    m.x.x = 1.0f; m.x.y = 0.0f; m.x.z = 0.0f; m.x.w = 0.0f;
    m.y.x = 0.0f; m.y.y = c;    m.y.z = s;    m.y.w = 0.0f;
    m.z.x = 0.0f; m.z.y = -s;   m.z.z = c;    m.z.w = 0.0f;
    m.w.x = 0.0f; m.w.y = 0.0f; m.w.z = 0.0f; m.w.w = 1.0f;
    return m;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::RotateY(T degrees)
{
    T radians = degrees * Pi / 180.0f;
    T s = std::sin(radians);
    T c = std::cos(radians);
    
    Matrix4<T> m;
    m.x.x = c;    m.x.y = 0.0f; m.x.z = -s;   m.x.w = 0.0f;
    m.y.x = 0.0f; m.y.y = 1.0f; m.y.z = 0.0f; m.y.w = 0.0f;
    m.z.x = s;    m.z.y = 0.0f; m.z.z = c;    m.z.w = 0.0f;
    m.w.x = 0.0f; m.w.y = 0.0f; m.w.z = 0.0f; m.w.w = 1.0f;
    return m;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::RotateZ(T degrees)
{
    T radians = degrees * Pi / 180.0f;
    T s = std::sin(radians);
    T c = std::cos(radians);
    
    Matrix4<T> m;
    m.x.x = c;    m.x.y = s;    m.x.z = 0.0f; m.x.w = 0.0f;
    m.y.x = -s;   m.y.y = c;    m.y.z = 0.0f; m.y.w = 0.0f;
    m.z.x = 0.0f; m.z.y = 0.0f; m.z.z = 1.0f; m.z.w = 0.0f;
    m.w.x = 0.0f; m.w.y = 0.0f; m.w.z = 0.0f; m.w.w = 1.0f;
    return m;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::Frustum(T left, T right, T bottom, T top, T near, T far)
{
    T a = 2 * near / (right - left);
    T b = 2 * near / (top - bottom);
    T c = (right + left) / (right - left);
    T d = (top + bottom) / (top - bottom);
    T e = -(far + near) / (far - near);
    T f = -2 * far * near / (far - near);
    
    Matrix4<T> m;
    m.x.x = a; m.x.y = 0; m.x.z = 0; m.x.w = 0;
    m.y.x = 0; m.y.y = b; m.y.z = 0; m.y.w = 0;
    m.z.x = c; m.z.y = d; m.z.z = e; m.z.w = -1;
    m.w.x = 0; m.w.y = 0; m.w.z = f; m.w.w = 0;
    return m;
}



typedef Matrix3<float> mat3;
typedef Matrix4<float> mat4;