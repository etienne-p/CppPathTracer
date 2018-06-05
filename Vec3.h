//
// Created by etienne cella on 2018-04-18.
//

#pragma once

#include <cmath>
#include <stdlib.h>

inline float rand01()
{
    return (float)rand() / (float)RAND_MAX;
}

inline float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

inline float rand(float a, float b)
{
    return lerp(a, b, rand01());
}

class Vec3
{
public:
    Vec3(){ v[0]= v[1]= v[2]=0; };
    Vec3(float x, float y, float z) { v[0]= x; v[1]= y; v[2]=z; }
    Vec3(const Vec3& v2) { v[0]= v2.x(); v[1]= v2.y(); v[2]= v2.z(); } // copy constructor

    inline float x() const { return v[0]; }
    inline float y() const { return v[1]; }
    inline float z() const { return v[2]; }

    inline float r() const { return v[0]; }
    inline float g() const { return v[1]; }
    inline float b() const { return v[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-()const { return Vec3(-v[0], -v[1], -v[2]); }
    inline float operator[](int i) const { return v[i]; }
    inline float& operator[](int i) { return v[i]; }

    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator+=(const float s);
    inline Vec3& operator-=(const float s);
    inline Vec3& operator*=(const float s);
    inline Vec3& operator/=(const float s);

    inline float squaredLength() const { return v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; }
    inline float length() const { return sqrtf(squaredLength()); }
    inline void normalize();

    static Vec3 one();
    static bool refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted);
    static Vec3 randomInUnitSphere();
    static Vec3 randomInUnitDisk();
    static float dot(const Vec3& v1, const Vec3& v2);
    static Vec3 lerp(const Vec3& v1, const Vec3& v2, float t);
    static Vec3 cross(const Vec3& v1, const Vec3& v2);
    static Vec3 normalize(const Vec3& v);
    static Vec3 reflect(const Vec3& v, const Vec3& n);

    float v[3];
};


inline Vec3& Vec3::operator+=(const Vec3 &v2)
{
    v[0]+= v2.v[0];
    v[1]+= v2.v[1];
    v[2]+= v2.v[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v2)
{
    v[0]-= v2.v[0];
    v[1]-= v2.v[1];
    v[2]-= v2.v[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v2)
{
    v[0]*= v2.v[0];
    v[1]*= v2.v[1];
    v[2]*= v2.v[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v2)
{
    v[0]/= v2.v[0];
    v[1]/= v2.v[1];
    v[2]/= v2.v[2];
    return *this;
}

inline Vec3& Vec3::operator+=(const float s)
{
    v[0]+= s;
    v[1]+= s;
    v[2]+= s;
    return *this;
}

inline Vec3& Vec3::operator-=(const float s)
{
    v[0]-= s;
    v[1]-= s;
    v[2]-= s;
    return *this;
}

inline Vec3& Vec3::operator*=(const float s)
{
    v[0]*= s;
    v[1]*= s;
    v[2]*= s;
    return *this;
}

inline Vec3& Vec3::operator/=(const float s)
{
    v[0]/= s;
    v[1]/= s;
    v[2]/= s;
    return *this;
}

inline Vec3 Vec3::one()
{
    return Vec3(1.0f, 1.0f, 1.0f);
}

inline void Vec3::normalize()
{
    float k = 1.0f / length();
    v[0] *= k; v[1] *= k; v[2] *= k;
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v1.v[2] * v2.v[2]);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2]);
}

inline Vec3 operator+(const Vec3& v1, float t)
{
    return Vec3(v1.v[0] + t, v1.v[1] + t, v1.v[2] + t);
}

inline Vec3 operator-(const Vec3& v1, float t)
{
    return Vec3(v1.v[0] - t, v1.v[1] - t, v1.v[2] - t);
}

inline Vec3 operator*(const Vec3& v1, float t)
{
    return Vec3(v1.v[0] * t, v1.v[1] * t, v1.v[2] * t);
}

inline Vec3 operator/(const Vec3& v1, float t)
{
    return Vec3(v1.v[0] / t, v1.v[1] / t, v1.v[2] / t);
}

inline Vec3 operator*(float t, const Vec3& v1)
{
    return Vec3(v1.v[0] * t, v1.v[1] * t, v1.v[2] * t);
}

inline Vec3 operator/(float t, const Vec3& v1)
{
    return Vec3(v1.v[0] / t, v1.v[1] / t, v1.v[2] / t);
}