#pragma once

namespace Pocket {
	class Point3
	{
	public:
		public:
        int x,y,z;
        
        inline Point3() { x = 0; y = 0; z = 0; }
        inline ~Point3() { }
        inline Point3(int v) { x = v; y = v; z = v; }
        inline Point3(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
        inline Point3(const Point3 &source) { x = source.x; y = source.y; z = source.z; }
                
        inline Point3 operator -() const {
            return Point3(-x,-y,-z);
        }
        
        inline Point3 operator +(const Point3 &other) const {
            return Point3(x + other.x, y + other.y, z + other.z);
        }
        
        inline Point3 operator -(const Point3 &other) const {
            return Point3(x - other.x, y - other.y, z - other.z);
        }
        
        inline Point3 operator *(const Point3 &other) const {
            return Point3(x * other.x, y * other.y, z * other.z);
        }
        
        inline Point3 operator / (const Point3 &other) const {
            return Point3(x / other.x, y / other.y, z / other.z);
        }
        
        inline Point3& operator +=(const Point3 &other) {
            x+=other.x;
            y+=other.y;
            z+=other.z;
            return *this;
        }
        
        inline Point3& operator -=(const Point3 &other) {
            x-=other.x;
            y-=other.y;
            z-=other.z;
            return *this;
        }
        
        inline Point3& operator *=(const Point3 &other) {
            x*=other.x;
            y*=other.y;
            z*=other.z;
            return *this;
        }
        
        inline Point3& operator /=(const Point3 &other) {
            x/=other.x;
            y/=other.y;
            z/=other.z;
            return *this;
        }
        
        inline Point3& operator *=(int s) {
            x*=s;
            y*=s;
            z*=s;
            return *this;
        }
        
        inline Point3& operator /=(int s) {
            x/=s;
            y/=s;
            z/=s;
            return *this;
        }
        
        inline bool operator ==(const Point3 &other) const{
            return !(x!=other.x || y!=other.y || z!=other.z);
        }
        
        inline bool operator !=(const Point3 &other) const{
            return (x!=other.x || y!=other.y || z!=other.z);
        }
        
        inline int operator[](int index) const{
            return (index == 0) ? x : ((index==1) ? y : z);
        }

	};
}
