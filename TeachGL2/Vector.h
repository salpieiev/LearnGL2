//
//  Vector.h
//  TeachGL
//
//  Created by Sergey Alpeev on 30.12.12.
//  Copyright (c) 2012 Brisk Mobile Inc. All rights reserved.
//

#pragma once
#include <cmath>



const float Pi = 4 * std::atan(1.0f);
const float TwoPi = 2 * Pi;



template <typename T>
class Vector2
{
public:
    Vector2();
    Vector2(T x, T y);
    
    Vector2<T> operator +(const Vector2 &vector) const;
    Vector2<T> operator -(const Vector2 &vector) const;
    Vector2<T> operator *(float s) const;
    Vector2<T> operator /(float s) const;
    
    template <typename P> P * Write(P *pData);
    
    T x;
    T y;
};

template <typename T>
class Vector3
{
public:
    Vector3();
    Vector3(T x, T y, T z);
    
    Vector3<T> operator -() const;
    Vector3<T> operator +(const Vector3 &vector) const;
    Vector3<T> operator -(const Vector3 &vector) const;
    
    float Length() const;
    void Normalize();
    Vector3<T> Normalized() const;
    Vector3<T> Cross(const Vector3<T> &v) const;
    template <typename P> P * Write(P *pData);
    
    T x;
    T y;
    T z;
};

template <typename T>
class Vector4
{
public:
    Vector4();
    Vector4(T x, T y, T z, T w);
    
    const T * Pointer() const;
    
    T x;
    T y;
    T z;
    T w;
};



template <typename T>
Vector2<T>::Vector2(): x(0), y(0)
{
    
}

template <typename T>
Vector2<T>::Vector2(T x, T y): x(x), y(y)
{
    
}

template <typename T>
Vector2<T> Vector2<T>::operator +(const Vector2 &vector) const
{
    return Vector2<T>(x + vector.x, y + vector.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator -(const Vector2 &vector) const
{
    return Vector2<T>(x - vector.x, y - vector.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator *(float s) const
{
    return Vector2<T>(x * s, y * s);
}

template <typename T>
Vector2<T> Vector2<T>::operator /(float s) const
{
    return Vector2<T>(x / s, y / s);
}

template <typename T>
template <typename P>
P * Vector2<T>::Write(P *pData)
{
    Vector2<T> *pVector = (Vector2<T> *)pData;
    *pVector++ = *this;
    return (P *)pVector;
}



template <typename T>
Vector3<T>::Vector3(): x(0), y(0), z(0)
{
    
}

template <typename T>
Vector3<T> Vector3<T>::operator-() const
{
    return Vector3<T>(-x, -y, -z);
}

template <typename T>
Vector3<T> Vector3<T>::operator +(const Vector3 &vector) const
{
    return Vector3<T>(x + vector.x, y + vector.y, z + vector.z);
}

template <typename T>
Vector3<T> Vector3<T>::operator -(const Vector3 &vector) const
{
    return Vector3<T>(x - vector.x, y - vector.y, z - vector.z);
}

template <typename T>
float Vector3<T>::Length() const
{
    return sqrt(x * x + y * y + z * z);
}

template <typename T>
void Vector3<T>::Normalize()
{
    float scale = 1.0f / Length();
    x *= scale;
    y *= scale;
    z *= scale;
}

template <typename T>
Vector3<T> Vector3<T>::Normalized() const
{
    Vector3<T> vector = *this;
    vector.Normalize();
    return vector;
}

template <typename T>
Vector3<T>::Vector3(T x, T y, T z): x(x), y(y), z(z)
{
    
}

template <typename T>
Vector3<T> Vector3<T>::Cross(const Vector3<T> &v) const
{
    Vector3<T> cross = Vector3<T>(y * v.z - z * v.y,
                                  z * v.x - x * v.z,
                                  x * v.y - y * v.x);
    return cross;
}

template <typename T>
template <typename P>
P * Vector3<T>::Write(P *pData)
{
    Vector3<T> *pVector = (Vector3<T> *)pData;
    *pVector++ = *this;
    return (P *)pVector;
}





template <typename T>
Vector4<T>::Vector4(): x(0), y(0), z(0), w(0)
{
    
}

template <typename T>
Vector4<T>::Vector4(T x, T y, T z, T w): x(x), y(y), z(z), w(w)
{
    
}

template <typename T>
const T * Vector4<T>::Pointer() const
{
    return &x;
}



typedef Vector2<int> ivec2;
typedef Vector2<float> vec2;
typedef Vector3<float> vec3;
typedef Vector4<float> vec4;