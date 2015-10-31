#pragma once
#include <math.h>
#include <sstream>
#include <iostream>

namespace Pocket {
    class Vector3;
	class Vector2 {

		public:
        float x,y;
        
        inline Vector2() { x = 0; y = 0; }
        inline ~Vector2() { }
        inline Vector2(float v) { x = v; y = v; }
        inline Vector2(float x, float y) { this->x = x; this->y = y; }
        inline Vector2(const Vector2 &source) { x = source.x; y = source.y; }
        Vector2(const Vector3& source);
        
        inline float SquareLength() const {
            return x * x + y * y;
        }
        
        inline float Length() const {
            return sqrt(x * x + y * y);
        }
        
        inline void Normalize() {
            float sqrLen = x * x + y * y;
            
            if (sqrLen>0) {
                sqrLen = 1.0f / sqrt(sqrLen);
                x *= sqrLen;
                y *= sqrLen;
            }
        }
        
        inline float Dot(const Pocket::Vector2 &other) const{
            return x * other.x + y * other.y;
        }
        
        inline Vector2 operator -() const {
            return Vector2(-x,-y);
        }
        
        inline Vector2 operator +(const Vector2 &other) const {
            return Vector2(x + other.x, y + other.y);
        }
        
        inline Vector2 operator -(const Vector2 &other) const {
            return Vector2(x - other.x, y - other.y);
        }
        
        inline Vector2 operator *(const Vector2 &other) const {
            return Vector2(x * other.x, y * other.y);
        }
        
        inline Vector2 operator / (const Vector2 &other) const {
            return Vector2(x / other.x, y / other.y);
        }
        
        inline Vector2& operator +=(const Vector2 &other) {
            x+=other.x;
            y+=other.y;
            return *this;
        }
        
        inline Vector2& operator -=(const Vector2 &other) {
            x-=other.x;
            y-=other.y;
            return *this;
        }
        
        inline Vector2& operator *=(const Vector2 &other) {
            x*=other.x;
            y*=other.y;
            return *this;
        }
        
        inline Vector2& operator /=(const Vector2 &other) {
            x/=other.x;
            y/=other.y;
            return *this;
        }
        
        inline Vector2& operator *=(float s) {
            x*=s;
            y*=s;
            return *this;
        }
        
        inline Vector2& operator /=(float s) {
            s = 1.0f / s;
            x*=s;
            y*=s;
            return *this;
        }
        
        inline bool operator ==(const Vector2 &other) const{
            return !(x!=other.x || y!=other.y);
        }
        
        inline  bool operator !=(const Vector2 &other) const{
            return (x!=other.x || y!=other.y);
        }
        
        inline float operator[](int index) const{
            return (index == 0) ? x : y;
        }

		inline float Aspect() const{
			if (y==0) return 1;
			return x / y;
		}
        
        inline Vector2 Normalized() const {
            Vector2 v(*this);
			v.Normalize();
			return v;
        }
        
        static inline Vector2 Lerp(const Vector2& source, const Vector2& dest, const float& t) {
            return Vector2(source.x + (dest.x - source.x) * t,
                           source.y + (dest.y - source.y) * t);
        }
        
        static Vector2 Bezier(const Vector2& position0, const Vector2& tangent0, const Vector2& position1, const Vector2& tangent1, const float& t);
        
        
        //void Serialize(TiXmlElement* xmlNode);
        //Vector2 Deserialize(TiXmlElement* xmlNode);
        
        Vector2 static Deserialize(const std::string& data, const char delim = ',');
    
        inline Vector2 Min(const Vector2& other) {
            return Vector2(x<other.x ? x : other.x, y<other.y ? y : other.y);
        }
        
        inline Vector2 Max(const Vector2& other) {
            return Vector2(x>other.x ? x : other.x, y>other.y ? y : other.y);
        }
        
        inline bool EqualEpsilon(const Vector2& other, const float epsilon = 0.0001f) {
            float abs = other.x - x;
            if (abs<0) abs = -abs;
            if (abs > epsilon) return false;
            abs = other.y - y;
            if (abs < 0) abs = -abs;
            if (abs > epsilon) return false;
            return true;
        }
    };
}

std::ostream& operator<<(std::ostream& stream, const Pocket::Vector2& v);
