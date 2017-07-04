//
//  Rect.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector2.hpp"

namespace Pocket {
    struct Rect {
        float x;
        float y;
        float width;
        float height;
  
        Rect() : x(0), y(0), width(0), height(0) {}
        Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
        Rect(const Vector2& position, const Vector2& size) : x(position.x), y(position.y), width(size.x), height(size.y) { }
        Rect(const Vector2& size) : x(0), y(0), width(size.x), height(size.y) { }
        
        
        inline Rect operator *(const Vector2 &vector) const {
            return Rect(x * vector.x, y * vector.y, width * vector.x, height * vector.y);
        }
        
        inline float Aspect() const {
            if (height == 0) return 1;
            return width / height;
        }
        
        friend std::ostream& operator<<(std::ostream& stream, const Pocket::Rect& r) {
            stream << r.x <<","<<r.y<<","<<r.width<<","<<r.height;
            return stream;
        }
        
        Rect static Deserialize(const std::string& data, const char delim = ',');
        
        inline bool operator ==(const Rect &other) const{
            return !(x!=other.x || y!=other.y || width!=other.width || height!=other.height);
        }
        
        inline  bool operator !=(const Rect &other) const{
            return (x!=other.x || y!=other.y || width!=other.width || height!=other.height);
        }
        
        inline Vector2 Position() const {
            return Vector2(x,y);
        }
        
        inline Vector2 Size() const {
            return Vector2(width, height);
        }
        
        inline Vector2 Center() const {
            return Vector2(x + width * 0.5f, y + height * 0.5f);
        }
        
        static Rect Lerp(const Rect& a, const Rect& b, float t);
    };
}