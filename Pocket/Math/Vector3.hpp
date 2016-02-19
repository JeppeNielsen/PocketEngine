//
//  Vector3.h
//  Components
//
//  Created by Jeppe Nielsen on 3/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#pragma once
#include <math.h>
#include "Vector2.hpp"

namespace Pocket {
    class Vector3 {
    public:
        
        inline Vector3() { x = y = z = 0; } 
        inline Vector3(float scales) { x = y = z = scales; }
        inline Vector3(float x, float y) {  this->x = x; this->y = y; z = 0; }
        inline Vector3(float x, float y, float z) {  this->x = x; this->y = y; this->z = z; }
        inline Vector3(const Vector3& other) {  x = other.x; y = other.y; z = other.z; }
        inline Vector3(const Vector2& other) {  x = other.x; y = other.y; z = 0; }
        inline Vector3(const Vector2& vector2, float z) {  x = vector2.x; y = vector2.y; this->z = z; }
        
        inline ~Vector3() {}
        
        inline float Length() const { return sqrtf(x*x+y*y+z*z); }
        inline float LengthSqaured() const { return x*x+y*y+z*z; }
        inline float Dot(const Vector3& other) const { return x*other.x+y*other.y+z*other.z; }
        inline void Normalize() {
            float length = x*x+y*y+z*z;
            if (length==0) return;
            length = 1.0f / sqrtf(length);
            x*=length;
            y*=length;
            z*=length;
        }


        
        // not faster on my iMac i7
        // carmacks fast InverseSquareRoot
        /*
         inline float InverseLength() {
         float length = x*x+y*y+z*z;
         long i;
         float x2, y;
         const float threehalfs = 1.5F;
         
         x2 = length * 0.5F;
         y  = length;
         i  = * ( long * ) &y;  // evil floating point bit level hacking
         i  = 0x5f3759df - ( i >> 1 ); // what the ****?
         y  = * ( float * ) &i;
         y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
         
         //assert( !isnan(y) );
         
         return y;
         }
         */
                
        float x;
        float y;
        float z;
        
        inline void operator =(const Vector3 &other) {
            x = other.x; y = other.y; z = other.z;
        }
        
        inline void operator =(const Vector2 &other) {
            x = other.x; y = other.y;
        }
        
        inline Vector3 operator -() const {
            return Vector3(-x,-y,-z);
        }
        
        inline Vector3 operator +(const Vector3 &other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        
        inline Vector3 operator -(const Vector3 &other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        
        inline Vector3 operator *(const Vector3 &other) const {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }
        
        inline Vector3 operator / (const Vector3 &other) const {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }
        
        inline Vector3& operator +=(const Vector3 &other) {
            x+=other.x;
            y+=other.y;
            z+=other.z;
            return *this;
        }
        
        inline Vector3& operator -=(const Vector3 &other) {
            x-=other.x;
            y-=other.y;
            z-=other.z;
            return *this;
        }
        
        inline Vector3& operator *=(const Vector3 &other) {
            x*=other.x;
            y*=other.y;
            z*=other.z;
            return *this;
        }
        
        inline Vector3& operator /=(const Vector3 &other) {
            x/=other.x;
            y/=other.y;
            z/=other.z;
            return *this;
        }
        
        inline Vector3& operator *=(float s) {
            x*=s;
            y*=s;
            z*=s;
            return *this;
        }
        
        inline Vector3& operator /=(float s) {
            s = 1.0f / s;
            x*=s;
            y*=s;
            z*=s;
            return *this;
        }
        
        inline bool operator ==(const Vector3 &other) {
            return !(x!=other.x || y!=other.y || z!=other.z);
        }
        
        inline bool operator !=(const Vector3 &other) {
            return (x!=other.x || y!=other.y || z!=other.z);
        }
        
        inline float operator [](const int index) {
            return *(&x + index);//(index == 0) ? x : ((index==1) ? y : z);
        }    
        
        inline Vector3 Cross( const Vector3& v ) const {
            return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }
        
        inline void Cross( const Vector3& v, Vector3& d ) const {
            d.x = y * v.z - z * v.y;
            d.y = z * v.x - x * v.z;
            d.z = x * v.y - y * v.x;
        }
        
        inline Vector3 Normalized() const {
            Vector3 v(*this);
			v.Normalize();
			return v;
        }
        
        static inline Vector3 Lerp(const Vector3& source, const Vector3& dest, const float& t) {
            return Vector3(source.x + (dest.x - source.x) * t,
                           source.y + (dest.y - source.y) * t,
                           source.z + (dest.z - source.z) * t);
        }
        
        static Vector3 Bezier(const Vector3& position0, const Vector3& tangent0, const Vector3& position1, const Vector3& tangent1, const float& t);
        
        Vector3 static Deserialize(const std::string& data, const char delim = ',');
        
        static Vector3 zero;
        
        inline Vector3 Min(const Vector3& other) {
            return Vector3(x<other.x ? x : other.x, y<other.y ? y : other.y, z<other.z ? z : other.z);
        }
        
        inline Vector3 Max(const Vector3& other) {
            return Vector3(x>other.x ? x : other.x, y>other.y ? y : other.y, z>other.z ? z : other.z);
        }
        
        friend std::ostream& operator<<(std::ostream& stream, const Pocket::Vector3& v) {
            stream << v.x <<","<<v.y<<","<<v.z;
            return stream;
        }
    };
}

