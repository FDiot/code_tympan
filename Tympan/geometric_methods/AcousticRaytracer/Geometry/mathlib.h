/* WARNING: I am an auto-generated header file, don't modify me !! */
/* Modify Tympan/models/common/mathlib.h instead */
/*
 * Copyright (C) <2012> <EDF-R&D> <FRANCE>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

//#include "std_tools.hpp"
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <vector>
#include <boost/foreach.hpp>

#include "3d.h"


/*! \file common/mathlib.h
    \brief Math library

    The vector type is declared here, and operations with this type.
    \author Projet_Tympan
*/
#ifndef __HMATHLIB__
#define __HMATHLIB__


/**
 * @brief Math functions
 */
namespace core_mathlib
{
// Activation or not (if commented) of targeting operations
//#define _ALLOW_TARGETING_ XXX


typedef float decimal;

typedef unsigned int bitSet; /*!< used to manage set of elements*/

#ifndef EPSILON_4 // was BARELY_EPSILON before
#define EPSILON_4           (decimal)0.0001 // 10e-4
#endif

#ifndef EPSILON_6
#define EPSILON_6           (decimal)0.000001 // 10e-6                      /*!< Approximation when comparing 2 decimal */
#endif

#ifndef EPSILON_15
#define EPSILON_15           (decimal)0.000000000000001 // 10e-15                      /*!< Approximation when comparing 2 decimal */
#endif

#ifndef M_PI
#define M_PI                (decimal)3.141592653589793238462643383279       /*!< PI */
#endif

#define M_PIDIV2            (decimal)1.570796326794896619231321691639       /*!< PI / 2 */

#ifndef M_2PI
#define M_2PI               (decimal)6.283185307179586476925286766559       /*!< 2 * PI */
#endif

#ifndef M_4PI
#define M_4PI               (decimal)12.566370614359172953850573533118      /*!< 4 * PI */
#endif

#define M_PI2               (decimal)9.869604401089358618834490999876       /*!< PI * PI */
#define M_PIDIV180          (decimal)0.01745329251994329576923690768488     /*!< PI / 180 */
#define M_180DIVPI          (decimal)57.295779513082320876798154814105      /*!< 180 / PI */

#define DegToRad(a) a*=M_PIDIV180
#define RadToDeg(a) a*=M_180DIVPI
#define RADIANS(a)  a*M_PIDIV180
#define DEGRES(a)   a*M_180DIVPI
#define ABS(x) (fabs(x))

#ifndef SIGN
#define SIGN(x) ((x) > 0 ? 1 : -1)
#endif


class vec2;
class vec4;
class ivec2;
class ivec3;
class ivec4;
/*****************************************************************************/
/*                                                                           */
/* vec3                                                                      */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief 3D vector
 * Vector defined with 3 float numbers
 */
template<typename base_t>
class base_vec3
{
public:
    base_vec3(void) : x(0), y(0), z(0) { }
    base_vec3(const base_t& _x, const base_t& _y, const base_t& _z) : x(_x), y(_y), z(_z) { }
    base_vec3(const base_t* _v) : x(_v[0]), y(_v[1]), z(_v[2]) { }
    base_vec3(const vec2& _v, base_t _z);
    base_vec3(const base_vec3& _v) : x(_v.x), y(_v.y), z(_v.z) { }
    base_vec3(const base_vec3* _v) : x(_v->x), y(_v->y), z(_v->z) { }
    base_vec3(const base_vec3& _v, const base_vec3& _w): x(_w.x - _v.x), y(_w.y - _v.y), z(_w.z - _v.z) {}                          //!< ab vector coordinates

    base_vec3(const vec4& _v);
    base_t get_x(){return this->x;}
    base_t get_y(){return this->y;}
    base_t get_z(){return this->z;}

    bool operator==(const base_vec3& _v) { return (fabs(this->x - _v.x) < EPSILON_6 && fabs(this->y - _v.y) < EPSILON_6 && fabs(this->z - _v.z) < EPSILON_6); }
    bool operator!=(const base_vec3& _v) { return !(*this == _v); }

    base_vec3& operator=(base_t _f) { this->x = _f; this->y = _f; this->z = _f; return (*this); }
    const base_vec3 operator*(base_t _f) const { return base_vec3(this->x * _f, this->y * _f, this->z * _f); }
    const base_vec3 operator/(base_t _f) const
    {
        if (fabs(_f) < EPSILON_6) { return *this; }
        _f = 1.0f / _f;
        return (*this) * _f;
    }
    base_vec3 operator/(const base_vec3& _v) const
    {
        return base_vec3(x / _v.x, y / _v.y, z / _v.z);
    }
    const base_vec3 operator+(const base_vec3& _v) const { return base_vec3(this->x + _v.x, this->y + _v.y, this->z + _v.z); }
    const base_vec3 operator-() const { return base_vec3(-this->x, -this->y, -this->z); }
    const base_vec3 operator-(const base_vec3& _v) const { return base_vec3(this->x - _v.x, this->y - _v.y, this->z - _v.z); }

    base_vec3& operator*=(base_t _f) { return *this = *this * _f; }
    base_vec3& operator/=(base_t _f) { return *this = *this / _f; }
    base_vec3& operator+=(const base_vec3& _v) { return *this = *this + _v; }
    base_vec3& operator-=(const base_vec3& _v) { return *this = *this - _v; }

    base_t operator*(const base_vec3& _v) const { return this->x * _v.x + this->y * _v.y + this->z * _v.z; }
    base_vec3 operator ^(const base_vec3& _v)const
    {
        return base_vec3(this->y * _v.z - this->z * _v.y,
                         this->z * _v.x - this->x * _v.z,
                         this->x * _v.y - this->y * _v.x) ;
    }   //!< cross product

    base_t operator*(const vec4& _v) const;



    operator base_t* () { return this->v; }
    operator const base_t* () const { return this->v; }
    base_t& operator[](int _i) { return this->v[_i]; }
    const base_t& operator[](int _i) const { return this->v[_i]; }

    bool barelyEqual(const base_vec3& _v) const  { return (fabs(this->x - _v.x) < 0.5 && fabs(this->y - _v.y) < 0.5 && fabs(this->z - _v.z) < 0.5); }
    void set(base_t _x, base_t _y, base_t _z) { this->x = _x; this->y = _y; this->z = _z; }
    void reset(void) { this->x = this->y = this->z = 0; }
    base_t length(void) const { return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z); }
    base_t normalize(void)
    {
        base_t inv, l = this->length();
        if (l < EPSILON_6) { return 0.0f; }
        inv = 1.0f / l;
        this->x *= inv;
        this->y *= inv;
        this->z *= inv;
        return l;
    }
    void cross(const base_vec3& v1, const base_vec3& v2)
    {
        this->x = v1.y * v2.z - v1.z * v2.y;
        this->y = v1.z * v2.x - v1.x * v2.z;
        this->z = v1.x * v2.y - v1.y * v2.x;
    }
    void cross(const base_vec3& v2)
    {
        base_t x = this->y * v2.z - this->z * v2.y;
        base_t y = this->z * v2.x - this->x * v2.z;
        this->z = this->x * v2.y - this->y * v2.x;
        this->y = y;
        this->x = x;
    }
    base_t cosinus(const base_vec3& ac)
    {
        return (this->x * ac.x + this->y * ac.y + this->z * ac.z) / (length() * ac.length());
    }
    base_t dot(const base_vec3& v) const { return ((this->x * v.x) + (this->y * v.y) + (this->z * v.z)); } //!< Scalar product
    // this < _v
    bool compare(const base_vec3& _v, base_t epsi = EPSILON_6) { return (fabs(this->x - _v.x) < epsi && fabs(this->y - _v.y) < epsi && fabs(this->z - _v.z) < epsi); }
    /** @brief Return the angle in radians between *this and v */
    base_t angle(const base_vec3& v)  const
    {
        base_t angle = acos(this->dot(v) / (this->length() * v.length()));
        if (angle < EPSILON_6) { return 0; }
        return angle;
    }
    /** @brief  Return the coordinates of the nearest point of *this on the line passing by vA and vB */
    base_vec3 closestPointOnLine(const base_vec3& vA, const base_vec3& vB) const { return (((vB - vA) * this->projectionOnLine(vA, vB)) + vA); }
    /** @brief  Return the coordinates of the nearest point of *this on the segment [vA,vB] */
    base_vec3 closestPointOnSegment(const base_vec3& vA, const base_vec3& vB) const
    {
        base_t factor = this->projectionOnLine(vA, vB);
        if (factor <= 0.0f) { return vA; }
        if (factor >= 1.0f) { return vB; }
        return (((vB - vA) * factor) + vA);
    }
    /** @brief  Return the projection factor of *this on the line passing by vA and vB */
    base_t projectionOnLine(const base_vec3& vA, const base_vec3& vB) const
    {
        base_vec3 v(vB - vA);
        return v.dot(*this - vA) / v.dot(v);
    }
    /** @brief Linear interpolation function between 2 vectors */
    base_vec3 lerp(base_vec3& u, base_vec3& v, base_t factor) { return ((u * (1 - factor)) + (v * factor)); }

    /**
     * @brief Compute the distance between two points pointed by *this and a_vector
     * @param a_vector A vector defining the second point
     * @return Distance
     */
    decimal distance(const base_vec3& a_vector) const
    {
        // compute distance between both points
        decimal dx = x - a_vector.x;
        decimal dy = y - a_vector.y;
        decimal dz = z - a_vector.z;

        // return result
        return(sqrt(dx * dx + dy * dy + dz * dz));
    }
    /** @brief Vector rotation
     *
     * Function to change the coordinates of the vector *this
     * by rotation around the vector n
     * with an angle (radians)
     *
     * @param[in] angle Angle in radians
     * @param[in] n Rotation vector
     */
    base_vec3 Rotation(const base_vec3& n, const base_t& angle) const
    {
        base_t m1 = cos(angle);
        base_t m2 = 1 - m1;
        base_t m3 = sin(angle);

        base_t m2nx = m2 * n.x;

        return base_vec3((m1 +   m2nx * n.x) * this->x + (m2nx * n.y -  m3 * n.z) * this->y + (m2nx * n.z +  m3 * n.y) * this->z,
                         (m2nx * n.y +  m3 * n.z) * this->x + (m1 +   m2 * n.y * n.y) * this->y + (m2 * n.y * n.z -  m3 * n.x) * this->z,
                         (m2nx * n.z -  m3 * n.y) * this->x + (m2 * n.y * n.z +  m3 * n.x) * this->y + (m1 +   m2 * n.z * n.z) * this->z);
    }
    union
    {
        struct {base_t x, y, z;};
        struct {base_t s, t, p;};
        struct {base_t r, g, b;};
        base_t v[3];
    };
};
typedef base_vec3<decimal> vec3;
typedef base_vec3<double> dvec3;

inline std::vector<vec3> operator * (const std::vector<vec3>& _v, const decimal& _a)
{
    std::vector<vec3> res ;
    BOOST_FOREACH(vec3 vec, _v) { res.push_back(vec * _a); }
    return res;
}

inline std::vector<vec3> operator + (const std::vector<vec3>& _u, const std::vector<vec3>& _v)
{
    assert(_u.size() == _v.size());
    std::vector<vec3> res;
    for (unsigned int i = 0; i < _v.size(); ++i) { res.push_back(_u[i] + _v[i]); }
    return res;
}

// Version double
inline std::vector<dvec3> operator * (const std::vector<dvec3>& _v, const decimal& _a)
{
    std::vector<dvec3> res ;
    BOOST_FOREACH(dvec3 vec, _v) { res.push_back(vec * _a); }
    return res;
}

inline std::vector<dvec3> operator + (const std::vector<dvec3>& _u, const std::vector<dvec3>& _v)
{
    assert(_u.size() == _v.size());
    std::vector<dvec3> res;
    for (unsigned int i = 0; i < _v.size(); ++i) { res.push_back(_u[i] + _v[i]); }
    return res;
}

/*!
 * \fn OPoint3D vec3toOPoint3D(const vec3& _v)
 * \brief Converts a vec3 to OPoint3D
 */
inline OPoint3D vec3toOPoint3D(const vec3& _v) { return OPoint3D(static_cast<double>(_v.x), static_cast<double>(_v.y), static_cast<double>(_v.z)); }

/*!
 * \fn vec3 OPoint3Dtovec3(const OPoint3D& _p)
 * \brief Converts a OPoint3D to vec3
 */
inline vec3 OPoint3Dtovec3(const OPoint3D& _p) { return vec3(static_cast<float>(_p._x), static_cast<float>(_p._y), static_cast<float>(_p._z)); }

/*!
 * \fn OVector3D vec3toOVector3D(const vec3& p)
 * \brief Converts a vec3 to OVector3D
 */
inline OVector3D vec3toOVector3D(const vec3& _v) { return OVector3D(static_cast<double>(_v.x), static_cast<double>(_v.y), static_cast<double>(_v.z)); }

/*!
 * \fn vec3 OVector3Dtovec3(const OVector3D& _p)
 * \brief Converts a OVector3D to vec3
 */
inline vec3 OVector3Dtovec3(const OVector3D& _v) { return vec3(static_cast<float>(_v._x), static_cast<float>(_v._y), static_cast<float>(_v._z)); }

/*****************************************************************************/
/*                                                                           */
/* vec2                                                                      */
/*                                                                           */
/*****************************************************************************/
/**
 * @brief 2D Vector
 * Vector defined with 2 float numbers
 */
class vec2
{
public:
    vec2(void) : x(0), y(0) { }
    vec2(decimal _x, decimal _y) : x(_x), y(_y) { }
    vec2(const decimal* _v) : x(_v[0]), y(_v[1]) { }
    vec2(const vec2& _v) : x(_v.x), y(_v.y) { }
    vec2(const vec2& _v, const vec2& _w): x(_w.x - _v.x), y(_w.y - _v.y)  {}
    vec2(const vec3& _v);
    vec2(const vec4& _v);

    int operator==(const vec2& _v) { return (fabs(this->x - _v.x) < EPSILON_6 && fabs(this->y - _v.y) < EPSILON_6); }

    int operator!=(const vec2& _v) { return !(*this == _v); }

    vec2& operator=(decimal _f) { this->x = _f; this->y = _f; return (*this); }
    const vec2 operator*(decimal _f) const { return vec2(this->x * _f, this->y * _f); }
    const vec2 operator/(decimal _f) const
    {
        if (fabs(_f) < EPSILON_6) { return *this; }
        _f = 1.0f / _f;
        return (*this) * _f;
    }
    const vec2 operator+(const vec2& _v) const { return vec2(this->x + _v.x, this->y + _v.y); }
    const vec2 operator-() const { return vec2(-this->x, -this->y); }
    const vec2 operator-(const vec2& _v) const { return vec2(this->x - _v.x, this->y - _v.y); }

    decimal operator^(const vec2& _v)const {return  this->x * _v.y - this->y * _v.x;} // produit mixte

    vec2& operator*=(decimal _f) { return *this = *this * _f; }
    vec2& operator/=(decimal _f) { return *this = *this / _f; }
    vec2& operator+=(const vec2& _v) { return *this = *this + _v; }
    vec2& operator-=(const vec2& _v) { return *this = *this - _v; }

    decimal operator*(const vec2& _v) const { return this->x * _v.x + this->y * _v.y; }

    operator decimal* () { return this->v; }
    operator const decimal* () const { return this->v; }
    decimal& operator[](int _i) { return this->v[_i]; }
    const decimal& operator[](int _i) const { return this->v[_i]; }


    void set(decimal _x, decimal _y) { this->x = _x; this->y = _y; }
    void reset(void) { this->x = this->y = 0; }
    decimal length(void) const { return sqrtf(this->x * this->x + this->y * this->y); }
    decimal normalize(void)
    {
        decimal inv, l = this->length();
        if (l < EPSILON_6) { return 0.0f; }
        inv = 1.0f / l;
        this->x *= inv;
        this->y *= inv;
        return l;
    }
    decimal det(vec2& v) { return (this->x * v.y - this->y * v.x); }  //!< 2D determinant
    decimal dot(const vec2& v) { return ((this->x * v.x) + (this->y * v.y)); } //!< Scalar product
    bool compare(const vec2& _v, decimal epsi = EPSILON_6) { return (fabs(this->x - _v.x) < epsi && fabs(this->y - _v.y) < epsi); }
    /** @brief Return the coordinates of the nearest point of *this on the line passing by vA and vB */
    vec2 closestPointOnLine(const vec2& vA, const vec2& vB) { return (((vB - vA) * this->projectionOnLine(vA, vB)) + vA); }
    /** @brief Return the coordinates of the nearest point of *this on the segment vA,vB */
    vec2 closestPointOnSegment(const vec2& vA, const vec2& vB)
    {
        decimal factor = this->projectionOnLine(vA, vB);
        if (factor <= 0.0f) { return vA; }
        if (factor >= 1.0f) { return vB; }
        return (((vB - vA) * factor) + vA);
    }
    /** @brief Return the projection factor of *this on the line passing by vA and vB */
    decimal projectionOnLine(const vec2& vA, const vec2& vB)
    {
        vec2 v(vB - vA);
        return v.dot(*this - vA) / v.dot(v);
    }
    /** @brief  Linear interpolation function between 2 vectors */
    vec2 lerp(vec2& u, vec2& v, decimal factor) { return ((u * (1 - factor)) + (v * factor)); }
    decimal angle(void) { return (decimal)atan2(this->y, this->x); }
    decimal angle(const vec2& v) { return (decimal)atan2(v.y - this->y, v.x - this->x); }

    union
    {
        struct {decimal x, y;};
        struct {decimal s, t;};
        decimal v[2];
    };
};

inline decimal area(const vec2& A, const vec2& B, const vec2& C) { return vec2(A, B) ^ vec2(A, C) * 0.5;}

inline void Cross(const vec3& v1, const vec3& v2, vec3& vout)
{
    vout.x = v1.y * v2.z - v1.z * v2.y;
    vout.y = v1.z * v2.x - v1.x * v2.z;
    vout.z = v1.x * v2.y - v1.y * v2.x;
}
inline vec3 Cross_r(const vec3& v1, const vec3& v2)
{
    return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
template<typename base_t>
inline base_vec3<base_t>::base_vec3(const vec2& _v, base_t _z)
    : x(_v.x), y(_v.y), z(_z)
{

}
inline vec2::vec2(const vec3& _v)
{
    this->x = _v.x;
    this->y = _v.y;
}

// This calculates a vector between 2 points and returns the result
inline void Vector(const vec3& vp1, const vec3& vp2, vec3& vout)
{

    vout.x = vp1.x - vp2.x;
    vout.y = vp1.y - vp2.y;
    vout.z = vp1.z - vp2.z;
}
inline vec3 Vector_r(const vec3& vp1, const vec3& vp2)
{
    return vec3(vp1.x - vp2.x, vp1.y - vp2.y, vp1.z - vp2.z);
}
/**
 * This calculates determinant 3*3
 */
inline decimal Determinant(const vec3& vp1, const vec3& vp2, const vec3& vp3)
{
    return vp1.x * vp2.y * vp3.z
           + vp1.y * vp2.z * vp3.x
           + vp1.z * vp2.x * vp3.y
           - vp1.x * vp2.z * vp3.y
           - vp1.y * vp2.x * vp3.z
           - vp1.z * vp2.y * vp3.x;
}

/*****************************************************************************/
/*                                                                           */
/* vec4                                                                      */
/*                                                                           */
/*****************************************************************************/

inline vec3 FaceNormal(const vec3& vp1, const vec3& vp2, const vec3& vp3)
{

    vec3 vret = Cross_r(Vector_r(vp1, vp2), Vector_r(vp2, vp3));
    vret.normalize();
    return vret;
}
/**
 * @brief 4D Vector
 * Vector defined with 4 float numbers
 */
class vec4
{
public:
    vec4(void) : x(0), y(0), z(0), w(1) { }
    vec4(decimal _x, decimal _y, decimal _z, decimal _w) : x(_x), y(_y), z(_z), w(_w) { }
    vec4(const decimal* _v) : x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3]) { }
    vec4(const vec3& _v) : x(_v.x), y(_v.y), z(_v.z), w(1) { }
    vec4(const vec3& _v, decimal _w) : x(_v.x), y(_v.y), z(_v.z), w(_w) { }
    vec4(const vec4& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) { }

    int operator==(const vec4& _v) { return (fabs(this->x - _v.x) < EPSILON_6 && fabs(this->y - _v.y) < EPSILON_6 && fabs(this->z - _v.z) < EPSILON_6 && fabs(this->w - _v.w) < EPSILON_6); }
    int operator!=(const vec4& _v) { return !(*this == _v); }

    vec4& operator=(decimal _f) { this->x = _f; this->y = _f; this->z = _f; this->w = _f; return (*this); }
    const vec4 operator*(decimal _f) const { return vec4(this->x * _f, this->y * _f, this->z * _f, this->w * _f); }
    const vec4 operator/(decimal _f) const
    {
        if (fabs(_f) < EPSILON_6) { return *this; }
        _f = 1.0f / _f;
        return (*this) * _f;
    }
    const vec4 operator+(const vec4& _v) const { return vec4(this->x + _v.x, this->y + _v.y, this->z + _v.z, this->w + _v.w); }
    const vec4 operator-() const { return vec4(-x, -y, -z, -w); }
    const vec4 operator-(const vec4& _v) const { return vec4(this->x - _v.x, this->y - _v.y, this->z - _v.z, this->w - _v.w); }

    vec4& operator*=(decimal _f) { return *this = *this * _f; }
    vec4& operator/=(decimal _f) { return *this = *this / _f; }
    vec4& operator+=(const vec4& _v) { return *this = *this + _v; }
    vec4& operator-=(const vec4& _v) { return *this = *this - _v; }

    decimal operator*(const vec3& _v) const { return this->x * _v.x + this->y * _v.y + this->z * _v.z + this->w; }
    decimal operator*(const vec4& _v) const { return this->x * _v.x + this->y * _v.y + this->z * _v.z + this->w * _v.w; }

    operator decimal* () { return this->v; }
    operator const decimal* () const { return this->v; }
    //  decimal &operator[](int _i) { return this->v[_i]; }
    //  const decimal &operator[](int _i) const { return this->v[_i]; }

    void set(decimal _x, decimal _y, decimal _z, decimal _w) { this->x = _x; this->y = _y; this->z = _z; this->w = _w; }
    void reset(void) { this->x = this->y = this->z = this->w = 0; }
    bool compare(const vec4& _v, decimal epsi = EPSILON_6) { return (fabs(this->x - _v.x) < epsi && fabs(this->y - _v.y) < epsi && fabs(this->z - _v.z) < epsi && fabs(this->w - _v.w) < epsi); }

    union
    {
        struct {decimal x, y, z, w;};
        struct {decimal s, t, p, q;};
        struct {decimal r, g, b, a;};
        struct {decimal fov, ratio, znear, zfar;};
        decimal v[4];
    };
};
/**
 * Calculate the determinant
 *
 * |x1 y1 z1 w1|
 * |x2 y2 z2 w2|
 * |x3 y3 z3 w3|
 * |x4 y4 z4 w4|
 */
inline decimal Determinant(const vec4& vp1, const vec4& vp2, const vec4& vp3, const vec4& vp4)
{
    return -(vp1.w * (vp2.x * (vp3.y * vp4.z - vp4.y * vp3.z) - vp3.x * (vp2.y * vp4.z - vp4.y * vp2.z) + vp4.x * (vp2.y * vp3.z - vp3.y * vp2.z)) - vp2.w * (vp1.x * (vp3.y * vp4.z - vp4.y * vp3.z) - vp3.x * (vp1.y * vp4.z - vp4.y * vp1.z) + vp4.x * (vp1.y * vp3.z - vp3.y * vp1.z)) + vp3.w * (vp1.x * (vp2.y * vp4.z - vp4.y * vp2.z) - vp2.x * (vp1.y * vp4.z - vp4.y * vp1.z) + vp4.x * (vp1.y * vp2.z - vp2.y * vp1.z)) - vp4.w * (vp1.x * (vp2.y * vp3.z - vp3.y * vp2.z) - vp2.x * (vp1.y * vp3.z - vp3.y * vp1.z) + vp3.x * (vp1.y * vp2.z - vp2.y * vp1.z)));
}
/**
 * Calculate the determinant
 *
 * |x1 y1 z1 1|
 * |x2 y2 z2 1|
 * |x3 y3 z3 1|
 * |x4 y4 z4 1|
 */
inline decimal Determinant(const vec3& vp1, const vec3& vp2, const vec3& vp3, const vec3& vp4)
{
    return vp1.x * (vp2.y * (vp3.z - vp4.z) - vp3.y * (vp2.z - vp4.z) + vp4.y * (vp2.z - vp3.z)) - vp2.x * (vp1.y * (vp3.z - vp4.z) - vp3.y * (vp1.z - vp4.z) + vp4.y * (vp1.z - vp3.z)) + vp3.x * (vp1.y * (vp2.z - vp4.z) - vp2.y * (vp1.z - vp4.z) + vp4.y * (vp1.z - vp2.z)) - vp4.x * (vp1.y * (vp2.z - vp3.z) - vp2.y * (vp1.z - vp3.z) + vp3.y * (vp1.z - vp2.z));
}

template<typename base_t>
inline base_vec3<base_t>::base_vec3(const vec4& _v)
{
    this->x = _v.x;
    this->y = _v.y;
    this->z = _v.z;
}
template<typename base_t>
inline base_t base_vec3<base_t>::operator*(const vec4& _v) const
{
    return this->x * _v.x + this->y * _v.y + this->z * _v.z + _v.w;
}


inline vec2::vec2(const vec4& _v)
{
    this->x = _v.x;
    this->y = _v.y;
}
inline bool colinear(const vec3& A, const vec3& B, const vec3& C, const decimal& aproximation)
{
    vec3 AB = B - A;
    vec3 AC = C - A;
    decimal diff = (AB / AB.length() - AC / AC.length()).length();
    return diff < aproximation;
}
/*****************************************************************************/
/*                                                                           */
/* ivec2                                                                     */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief 2D Vector
 * Vector defined with 2 integers
 */
class ivec2
{
public:
    ivec2(void) : a(0), b(0) { }
    ivec2(long _a, long _b) : a(_a), b(_b) { }
    ivec2(const long* iv) : a(iv[0]), b(iv[1]) { }
    ivec2(const ivec2& iv) : a(iv.a), b(iv.b) { }

    int operator==(const ivec2& iv) { return ((this->a == iv.a) && (this->b == iv.b)); }
    int operator!=(const ivec2& iv) { return !(*this == iv); }

    ivec2& operator=(long _i) { this->x = _i; this->y = _i; return (*this); }
    const ivec2 operator*(long _i) const { return ivec2(this->a * _i, this->b * _i); }
    const ivec2 operator/(long _i) const { return ivec2(this->a / _i, this->b / _i); }
    const ivec2 operator+(const ivec2& iv) const { return ivec2(this->a + iv.a, this->b + iv.b); }
    const ivec2 operator-() const { return ivec2(-this->a, -this->b); }
    const ivec2 operator-(const ivec2& iv) const { return ivec2(this->a - iv.a, this->b - iv.b); }

    ivec2& operator*=(long _i) { return *this = *this * _i; }
    ivec2& operator/=(long _i) { return *this = *this / _i; }
    ivec2& operator+=(const ivec2& iv) { return *this = *this + iv; }
    ivec2& operator-=(const ivec2& iv) { return *this = *this - iv; }

    long operator*(const ivec2& iv) const { return this->a * iv.a + this->b * iv.b; }

    operator long* () { return this->i; }
    operator const long* () const { return this->i; }
    //  long &operator[](int _i) { return this->i[_i]; }
    //  const long &operator[](int _i) const { return this->i[_i]; }

    void set(long _a, long _b) { this->a = _a; this->b = _b; }
    void reset(void) { this->a = this->b = 0; }
    void swap(ivec2& iv) { long tmp = a; a = iv.a; iv.a = tmp; tmp = b; b = iv.b; iv.b = tmp; }
    void swap(ivec2* iv) { this->swap(*iv); }

    union
    {
        struct {long a, b;};
        struct {long x, y;};
        long i[2];
    };
};

/*****************************************************************************/
/*                                                                           */
/* ivec3                                                                     */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief 3D Vector
 * Vector defined with 3 integers
 */
class ivec3
{
public:
    ivec3(void) : a(0), b(0), c(0) { }
    ivec3(long _a, long _b, long _c) : a(_a), b(_b), c(_c) { }
    ivec3(const long* iv) : a(iv[0]), b(iv[1]), c(iv[2]) { }
    ivec3(const ivec3& iv) : a(iv.a), b(iv.b), c(iv.c) { }
    ivec3(const ivec4& iv);

    int operator==(const ivec3& iv) { return ((this->a == iv.a) && (this->b == iv.b) && (this->c == iv.c)); }
    int operator!=(const ivec3& iv) { return !(*this == iv); }

    ivec3& operator=(long _i) { this->x = _i; this->y = _i; this->z = _i; return (*this); }
    ivec3& operator=(vec3& iv) { this->a = (long)iv[0]; this->b = (long)iv[1]; this->c = (long)iv[2]; return (*this); }
    const ivec3 operator*(long _i) const { return ivec3(this->a * _i, this->b * _i, this->c * _i); }
    const ivec3 operator/(long _i) const { return ivec3(this->a / _i, this->b / _i, this->c / _i); }
    const ivec3 operator+(const ivec3& iv) const { return ivec3(this->a + iv.a, this->b + iv.b, this->c + iv.c); }
    const ivec3 operator-() const { return ivec3(-this->a, -this->b, -this->c); }
    const ivec3 operator-(const ivec3& iv) const { return ivec3(this->a - iv.a, this->b - iv.b, this->c - iv.c); }

    ivec3& operator*=(long _i) { return *this = *this * _i; }
    ivec3& operator/=(long _i) { return *this = *this / _i; }
    ivec3& operator+=(const ivec3& iv) { return *this = *this + iv; }
    ivec3& operator-=(const ivec3& iv) { return *this = *this - iv; }

    long operator*(const ivec3& iv) const { return this->a * iv.a + this->b * iv.b + this->c * iv.c; }
    long operator*(const ivec4& iv) const;

    operator long* () { return this->i; }
    operator const long* () const { return this->i; }
    //  long &operator[](int _i) { return this->i[_i]; }
    //  const long &operator[](int _i) const { return this->i[_i]; }

    void set(long _a, long _b, long _c) { this->a = _a; this->b = _b; this->c = _c; }
    void set(int tab[3]) { this->a = tab[0]; this->b = tab[1]; this->c = tab[2]; }
    void reset(void) { this->a = this->b = this->c = 0; }
    void swap(ivec3& iv) { long tmp = a; a = iv.a; iv.a = tmp; tmp = b; b = iv.b; iv.b = tmp; tmp = c; c = iv.c; iv.c = tmp; }
    void swap(ivec3* iv) { this->swap(*iv); }

    union
    {
        struct {long a, b, c;};
        struct {long x, y, z;};
        struct {long red, green, blue;};
        long i[3];
    };
};

/*****************************************************************************/
/*                                                                           */
/* ivec4                                                                     */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief 4D Vector
 * Vector defined with 4 integers
 */
class ivec4
{
public:
    ivec4(void) : a(0), b(0), c(0), d(1) { }
    ivec4(long _a, long _b, long _c, long _d) : a(_a), b(_b), c(_c), d(_d) { }
    ivec4(const long* iv) : a(iv[0]), b(iv[1]), c(iv[2]), d(iv[3]) { }
    ivec4(const ivec3& iv) : a(iv.a), b(iv.b), c(iv.c), d(1) { }
    ivec4(const ivec3& iv, long _d) : a(iv.a), b(iv.b), c(iv.c), d(_d) { }
    ivec4(const ivec4& iv) : a(iv.a), b(iv.b), c(iv.c), d(iv.d) { }

    int operator==(const ivec4& iv) { return ((this->a == iv.a) && (this->b == iv.b) && (this->c == iv.c) && (this->d == iv.d)); }
    int operator!=(const ivec4& iv) { return !(*this == iv); }

    ivec4& operator=(long _i) { this->x = _i; this->y = _i; this->z = _i; this->w = _i; return (*this); }
    const ivec4 operator*(long _i) const { return ivec4(this->a * _i, this->b * _i, this->c * _i, this->d * _i); }
    const ivec4 operator/(long _i) const { return ivec4(this->a / _i, this->b / _i, this->c / _i, this->d / _i); }
    const ivec4 operator+(const ivec4& iv) const { return ivec4(this->a + iv.a, this->b + iv.b, this->c + iv.c, this->d + iv.d); }
    const ivec4 operator-() const { return ivec4(-this->a, -this->b, -this->c, -this->d); }
    const ivec4 operator-(const ivec4& iv) const { return ivec4(this->a - iv.a, this->b - iv.b, this->c - iv.c, this->d - iv.d); }

    ivec4& operator*=(long _i) { return *this = *this * _i; }
    ivec4& operator/=(long _i) { return *this = *this / _i; }
    ivec4& operator+=(const ivec4& iv) { return *this = *this + iv; }
    ivec4& operator-=(const ivec4& iv) { return *this = *this - iv; }

    long operator*(const ivec3& iv) const { return this->a * iv.a + this->b * iv.b + this->c * iv.c + this->d; }
    long operator*(const ivec4& iv) const { return this->a * iv.a + this->b * iv.b + this->c * iv.c + this->d * iv.d; }

    operator long* () { return this->i; }
    operator const long* () const { return this->i; }
    //  long &operator[](int _i) { return this->i[_i]; }
    //  const long &operator[](int _i) const { return this->i[_i]; }

    void set(long _a, long _b, long _c, long _d) { this->a = _a; this->b = _b; this->c = _c; this->d = _d; }
    void reset(void) { this->a = this->b = this->c = this->d = 0; }
    void swap(ivec4& iv) { long tmp = a; a = iv.a; iv.a = tmp; tmp = b; b = iv.b; iv.b = tmp; tmp = c; c = iv.c; iv.c = tmp; tmp = d; d = iv.d; iv.d = tmp; }
    void swap(ivec4* iv) { this->swap(*iv); }

    union
    {
        struct {long a, b, c, d;};
        struct {long x, y, z, w;};
        struct {long red, green, blue, alpha;};
        long i[4];
    };
};

inline ivec3::ivec3(const ivec4& iv)
{
    this->a = iv.a;
    this->b = iv.b;
    this->c = iv.c;
}

inline long ivec3::operator*(const ivec4& iv) const
{
    return this->a * iv.a + this->b * iv.b + this->c * iv.c + iv.d;
}


inline decimal CalcTetraVolume(vec3 A, vec3 B, vec3 C, vec3 D)
{
    A -= D;
    B -= D;
    C -= D;
    return fabs((A.dot(Cross_r(B, C))) / 6.f);
}
/**
 * @return Return the central point of a triangle
 */
inline vec3 GetGTriangle(const vec3& A, const vec3& B, const vec3& C)
{
    vec3 I = (B + C) / 2;
    vec3 AG = (I - A) * (2.f / 3.f);
    return A + AG;
}
inline vec3 GetGTetra(const vec3& A, const vec3& B, const vec3& C, const vec3& D)
{
    return (A + B + C + D) / 4;
}
inline decimal GetAireTriangle(const vec3& a, const vec3& b, const vec3& c)
{
    vec3 ab;
    vec3 ac;
    Vector(a, b, ab);
    Vector(a, c, ac);
    ab.cross(ac);
    return .5f * ab.length();
}

inline float Clamp(float val, float low, float high)
{
    if (val < low) { return low; }
    else if (val > high) { return high; }
    else { return val; }
}


inline int Clamp(int val, int low, int high)
{
    if (val < low) { return low; }
    else if (val > high) { return high; }
    else { return val; }
}

inline int Floor2Int(float val)
{
    return (int)floorf(val);
}

inline int Round2Int(float val)
{
    return Floor2Int(val + 0.5f);
}
/*
 * @param ecart Au maximum 2*PI au minimum 0
 * @return Vrai si Si ecart==0 ou ecart>0.1
 */
/*inline bool DotIsInVertex(const vec3& dot,const vec3& va,const vec3& vb,const vec3& vc,decimal* ecart=NULL)
{ //retourne vrai si le point est dans un triangle, le total a la fin correspond a environ 2PI si c'est le cas
    decimal totangle=0;
    // calcul des vecteurs des cotes
    vec3 vda(dot-va);
    vec3 vdb(dot-vb);
    vec3 vdc(dot-vc);
    //Calcul de la somme des angles sur le plan xy
    totangle+=vda.angle(vdb);
    totangle+=vda.angle(vdc);
    totangle+=vdb.angle(vdc);
    if(ecart)
        *ecart=fabs(M_2PI-totangle);
    if(int(totangle*10)==int(M_2PI*10) || !st_isfinite(totangle))
        return true;
    else
        return false;
}*/
/*
 * Retourne vrai si le point dotTest est dans le tetraedre forme par les sommets v1,v2,v3,v4
 * @ref http://steve.hollasch.net/cgindex/geometry/ptintet.html
 */
/*static bool DotInTetra(const vec3& dotTest,const vec3& v1,const vec3& v2,const vec3& v3,const vec3& v4)
{
    int sd0=SIGN(Determinant(v1,v2,v3,v4));
    if(SIGN(Determinant(dotTest,v2,v3,v4))!=sd0)
        return false;
    if(SIGN(Determinant(v1,dotTest,v3,v4))!=sd0)
        return false;
    if(SIGN(Determinant(v1,v2,dotTest,v4))!=sd0)
        return false;
    if(SIGN(Determinant(v1,v2,v3,dotTest))!=sd0)
        return false;
    return true;
}*/

/**
 * \brief Calculate the segment between the lines (p1,p2) and (p3,p4)
 * @param p1 first point of the first segment
 * @param p2 second point of the first segment
 * @param p3 first point of the second segment
 * @param p4 second point of the second segment
 * @param pa point of the first line (p1p2) that belongs to the shortest segment between the two lines 
 * @param pb point of the second line (p3p4) that belongs to the shortest segment between the two lines 
 * @param mua position of a along the first line (p1p2)
 * @param mub position of b along the second line (p3p4)
 * @return False if no solution is found
 */
inline bool LineLineIntersect(
    const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4, vec3* pa, vec3* pb,
    decimal* mua, decimal* mub)
{
    vec3 w, v, u;
    decimal vw, uv, uw, vv, uu;
    decimal numer, denom;

    //compute the vector u defined by the first segment
    u.x = p2.x - p1.x;
    u.y = p2.y - p1.y;
    u.z = p2.z - p1.z;

    //p1 and p2 coincide, the first segment is reduced to a point. 
    if (fabs(u.x)  < EPSILON_6 && fabs(u.y)  < EPSILON_6 && fabs(u.z)  < EPSILON_6)
    {
        return false;
    }
    
    //compute the vector v defined by the second segment
    v.x = p4.x - p3.x;
    v.y = p4.y - p3.y;
    v.z = p4.z - p3.z;

    //p4 and p3 coincide, the second segment is reduced to a point. 
    if (fabs(v.x)  < EPSILON_6 && fabs(v.y)  < EPSILON_6 && fabs(v.z)  < EPSILON_6)
    {
        return false;
    }

    //Idea: Find pa and pb such that (pa-pb)*u=(pa-pb)*v=0 (1) (because the shortest segment between two lines is perpendicular to both of them)
    //      With pa=p1+mua*u and pb=p3+mub*v               (2) (pa is at mua times u along the line p1p2)
    //
    //      Expanding (1) with (2) we get: 
    //      (w + mua*u - mub*v)*u=0 and (w + mua*u - mub*v)*v=0 
    //
    //      which can be further expanded into:
    //      wu+mua*uu-mub*uv=0 and wv+mua*uv-mub*vv=0
    //
    //      Solving for mua : mua = (uv*vw - vv*uw)/(uu*vv - uv*uv)
    //
    //      Back-substituting gives mub = (vw + mua*uv)/vv = (uu*vw - uv*uw)/(uu*vv - uv*uv)

    //Compute the vector w formed by the first vertices of both segments
    w.x = p1.x - p3.x;
    w.y = p1.y - p3.y;
    w.z = p1.z - p3.z;

    // Pre-compute u*u, v*v, u*v, u*w and v*w
    vw = v*w;
    uv = u*v;
    uw = u*w;
    vv = v*v;
    uu = u*u;

    //Compute the denominator
    denom = uu*vv - uv*uv;

    //If the denominator is 0, then the two equations are dependant and therefore the two lines are parallel
    if (fabs(denom) < EPSILON_6)
    {
        return false;
    }

    //Compute the numerator of mua
    numer = uv*vw - vv*uw;

    //Compute mua and mub
    *mua = numer / denom;
    *mub = (vw + uv * (*mua)) / vv;

    pa->x = p1.x + *mua * u.x;
    pa->y = p1.y + *mua * u.y;
    pa->z = p1.z + *mua * u.z;
    pb->x = p3.x + *mub * v.x;
    pb->y = p3.y + *mub * v.y;
    pb->z = p3.z + *mub * v.z;

    return true;
}

inline bool pointInPolygone(const vec2& p, const std::vector<vec2>& points)
{
    int i, j;
    bool c = false;
    for (i = 0, j = points.size() - 1; i < (int)points.size(); j = i++)
    {
        if ((((points.at(i).y <= p.y) && (p.y < points.at(j).y)) ||
             ((points.at(j).y <= p.y) && (p.y < points.at(i).y))) &&
            (p.x < (points.at(j).x - points.at(i).x) * (p.y - points.at(i).y) / (points.at(j).y - points.at(i).y) + points.at(i).x))
        {
            c = !c;
        }
    }
    return c;
}

//const Vector& P, decimal* pfSParam, decimal* pfTParam
/**
 * Closest distance between a point and a triangle.
 * @param va Vertex A of the triangle
 * @param vb Vertex B of the triangle
 * @param vc Vertex C of the triangle
 * @param P Point
 * @param pfSParam  Barycentric coordinate of triangle at point closest to p (u)
 * @param pfTParam  Barycentric coordinate of triangle at point closest to p (v)
 * @return Shortest distance squared.
 *
 * The third Barycentric coordinate is implicit, ie. w = 1.0 - u - v
 *
 * Taken from:
 * geometrictools.com
 * @see http://www.geometrictools.com/LibFoundation/Distance/Distance.html
 */

inline decimal ClosestDistanceBetweenDotAndTriangle(
    const vec3& va, const vec3& vb, const vec3& vc, //Triangle
    const vec3& P,
    decimal* pfSParam,
    decimal* pfTParam
)
{
    //-------------------------------------------------------------------
    // 2 edges of the triangle
    //-------------------------------------------------------------------
    vec3 E0 = (vb - va);
    vec3 E1 = (vc - va);

    vec3 kDiff = (va - P);
    decimal fA00 = E0 * E0;
    decimal fA01 = E0 * E1;
    decimal fA11 = E1 * E1;
    decimal fB0  = kDiff * E0;
    decimal fB1  = kDiff * E1;
    decimal fC   = kDiff * kDiff;
    decimal fDet = (decimal) fabs(fA00 * fA11 - fA01 * fA01);
    decimal fS   = fA01 * fB1 - fA11 * fB0;
    decimal fT   = fA01 * fB0 - fA00 * fB1;
    decimal fSqrDist;

    if (fabs(fDet) < 0.00000001f)
    {
        return 100000000.0f;
    }

    if (fS + fT <= fDet)
    {
        if (fS < (decimal)0.0)
        {
            if (fT < (decimal)0.0)    // region 4
            {
                if (fB0 < (decimal)0.0)
                {
                    fT = (decimal)0.0;
                    if (-fB0 >= fA00)
                    {
                        fS = (decimal)1.0;
                        fSqrDist = fA00 + ((decimal)2.0) * fB0 + fC;
                    }
                    else
                    {
                        fS = -fB0 / fA00;
                        fSqrDist = fB0 * fS + fC;
                    }
                }
                else
                {
                    fS = (decimal)0.0;
                    if (fB1 >= (decimal)0.0)
                    {
                        fT = (decimal)0.0;
                        fSqrDist = fC;
                    }
                    else if (-fB1 >= fA11)
                    {
                        fT = (decimal)1.0;
                        fSqrDist = fA11 + ((decimal)2.0) * fB1 + fC;
                    }
                    else
                    {
                        fT = -fB1 / fA11;
                        fSqrDist = fB1 * fT + fC;
                    }
                }
            }
            else  // region 3
            {
                fS = (decimal)0.0;
                if (fB1 >= (decimal)0.0)
                {
                    fT = (decimal)0.0;
                    fSqrDist = fC;
                }
                else if (-fB1 >= fA11)
                {
                    fT = (decimal)1.0;
                    fSqrDist = fA11 + ((decimal)2.0) * fB1 + fC;
                }
                else
                {
                    fT = -fB1 / fA11;
                    fSqrDist = fB1 * fT + fC;
                }
            }
        }
        else if (fT < (decimal)0.0)    // region 5
        {
            fT = (decimal)0.0;
            if (fB0 >= (decimal)0.0)
            {
                fS = (decimal)0.0;
                fSqrDist = fC;
            }
            else if (-fB0 >= fA00)
            {
                fS = (decimal)1.0;
                fSqrDist = fA00 + ((decimal)2.0) * fB0 + fC;
            }
            else
            {
                fS = -fB0 / fA00;
                fSqrDist = fB0 * fS + fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            decimal fInvDet = ((decimal)1.0) / fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS * (fA00 * fS + fA01 * fT + ((decimal)2.0) * fB0) +
                       fT * (fA01 * fS + fA11 * fT + ((decimal)2.0) * fB1) + fC;
        }
    }
    else
    {
        decimal fTmp0, fTmp1, fNumer, fDenom;

        if (fS < (decimal)0.0)    // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if (fNumer >= fDenom)
                {
                    fS = (decimal)1.0;
                    fT = (decimal)0.0;
                    fSqrDist = fA00 + ((decimal)2.0) * fB0 + fC;
                }
                else
                {
                    fS = fNumer / fDenom;
                    fT = (decimal)1.0 - fS;
                    fSqrDist = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                               fT * (fA01 * fS + fA11 * fT + ((decimal)2.0) * fB1) + fC;
                }
            }
            else
            {
                fS = (decimal)0.0;
                if (fTmp1 <= (decimal)0.0)
                {
                    fT = (decimal)1.0;
                    fSqrDist = fA11 + ((decimal)2.0) * fB1 + fC;
                }
                else if (fB1 >= (decimal)0.0)
                {
                    fT = (decimal)0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fT = -fB1 / fA11;
                    fSqrDist = fB1 * fT + fC;
                }
            }
        }
        else if (fT < (decimal)0.0)    // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - ((decimal)2.0) * fA01 + fA11;
                if (fNumer >= fDenom)
                {
                    fT = (decimal)1.0;
                    fS = (decimal)0.0;
                    fSqrDist = fA11 + ((decimal)2.0) * fB1 + fC;
                }
                else
                {
                    fT = fNumer / fDenom;
                    fS = (decimal)1.0 - fT;
                    fSqrDist = fS * (fA00 * fS + fA01 * fT + ((decimal)2.0) * fB0) +
                               fT * (fA01 * fS + fA11 * fT + ((decimal)2.0) * fB1) + fC;
                }
            }
            else
            {
                fT = (decimal)0.0;
                if (fTmp1 <= (decimal)0.0)
                {
                    fS = (decimal)1.0;
                    fSqrDist = fA00 + ((decimal)2.0) * fB0 + fC;
                }
                else if (fB0 >= (decimal)0.0)
                {
                    fS = (decimal)0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0 / fA00;
                    fSqrDist = fB0 * fS + fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if (fNumer <= (decimal)0.0)
            {
                fS = (decimal)0.0;
                fT = (decimal)1.0;
                fSqrDist = fA11 + ((decimal)2.0) * fB1 + fC;
            }
            else
            {
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if (fNumer >= fDenom)
                {
                    fS = (decimal)1.0;
                    fT = (decimal)0.0;
                    fSqrDist = fA00 + ((decimal)2.0) * fB0 + fC;
                }
                else
                {
                    fS = fNumer / fDenom;
                    fT = (decimal)1.0 - fS;
                    fSqrDist = fS * (fA00 * fS + fA01 * fT + ((decimal)2.0) * fB0) +
                               fT * (fA01 * fS + fA11 * fT + ((decimal)2.0) * fB1) + fC;
                }
            }
        }
    }

    if (pfSParam)
    {
        *pfSParam = fS;
    }

    if (pfTParam)
    {
        *pfTParam = fT;
    }

    return (decimal) fabs(fSqrDist);
}

/*
 * \brief binary operation
 */

/*!
 * \fn unsigned int buildBitSet(const unsigned int length)
 * \brief build a bit sequence set to 1, for length bits
 */
inline unsigned int buildBitSet(const unsigned short& length)
{
    unsigned int res = 1;
    for (unsigned short i = 0; i < length - 1; i++, res++) { res = res << 1; }
    return res;
}

/*
 * \fn sequence buildComplementaryBitSet(const unsigned& int length, const unsigned int& bitSet )
 * \brief build the bit sequence representing complementary of bitset, taking account of is real useful length
 */
inline unsigned int buildComplementaryBitSet(const unsigned int& length, const unsigned int& bitSet)
{
    return (bitSet ^ buildBitSet(length));
}

}; //fin namespace
using namespace core_mathlib;
#endif // __HMATHLIB__
