#pragma once

namespace Pocket {
	class Point
	{
	public:
		public:
        int x,y;
        
        inline Point() { x = 0; y = 0; }
        inline ~Point() { }
        inline Point(int v) { x = v; y = v; }
        inline Point(int x, int y) { this->x = x; this->y = y; }
        inline Point(const Point &source) { x = source.x; y = source.y; }
                
        inline Point operator -() const {
            return Point(-x,-y);
        }
        
        inline Point operator +(const Point &other) const {
            return Point(x + other.x, y + other.y);
        }
        
        inline Point operator -(const Point &other) const {
            return Point(x - other.x, y - other.y);
        }
        
        inline Point operator *(const Point &other) const {
            return Point(x * other.x, y * other.y);
        }
        
        inline Point operator / (const Point &other) const {
            return Point(x / other.x, y / other.y);
        }
        
        inline Point& operator +=(const Point &other) {
            x+=other.x;
            y+=other.y;
            return *this;
        }
        
        inline Point& operator -=(const Point &other) {
            x-=other.x;
            y-=other.y;
            return *this;
        }
        
        inline Point& operator *=(const Point &other) {
            x*=other.x;
            y*=other.y;
            return *this;
        }
        
        inline Point& operator /=(const Point &other) {
            x/=other.x;
            y/=other.y;
            return *this;
        }
        
        inline Point& operator *=(int s) {
            x*=s;
            y*=s;
            return *this;
        }
        
        inline Point& operator /=(int s) {
            x/=s;
            y/=s;
            return *this;
        }
        
        inline bool operator ==(const Point &other) const{
            return !(x!=other.x || y!=other.y);
        }
        
        inline bool operator !=(const Point &other) const{
            return (x!=other.x || y!=other.y);
        }
        
        inline int operator[](int index) const{
            return (index == 0) ? x : y;
        }

	};
}
