//
//  NavMesh.h
//  Homeland
//
//  Created by Jeppe Nielsen on 17/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector2.hpp"
#include <vector>

using namespace Pocket;

struct NavTriangle {
    Vector2 corners[3];
    NavTriangle* neighbors[3];
    inline bool hasCorner(const Vector2& corner, int& cornerIndex) {
        if (corners[0].EqualEpsilon(corner)) {
            cornerIndex = 0;
            return true;
        }
        if (corners[1].EqualEpsilon(corner)) {
            cornerIndex = 1;
            return true;
        }
        if (corners[2].EqualEpsilon(corner)) {
            cornerIndex = 2;
            return true;
        }
        return false;
    }
    
    inline int FindNeighborIndex(NavTriangle* neighbor) {
        for (int i=0; i<3; i++) {
            if (neighbors[i]==neighbor) return i;
        }
        return -1;
    }
    
    inline Vector2 GetMidpoint(int neighborIndex) {
        if (neighborIndex<2) {
            return (corners[neighborIndex] + corners[neighborIndex + 1]) * 0.5f;
        } else {
            return (corners[2] + corners[0]) * 0.5f;
        }
    }
    
    void Reset() {
        pathID = 0;
        h = 0;
        g = 0;
        total = 0;
        parent = 0;
        position = 0;
    }
    int index;
    float h;
    float g;
    float total;
    NavTriangle* parent;
    Vector2 position;
    int pathID;
    
    
    
    inline float side(const Vector2& p1, const Vector2& p2, const Vector2& p) {
        return (p2.y - p1.y) * (p.x - p1.x) + (-p2.x + p1.x)*(p.y - p1.y);
    }
    
    bool PointInside(const Vector2& p) {
        if (side(corners[0], corners[1], p)>0) return false;
        if (side(corners[1], corners[2], p)>0) return false;
        if (side(corners[2], corners[0], p)>0) return false;
        return true;
    
    
        /*
        
            function side(x1, y1, x2, y2, x, y:Number):Number
            {
            return (y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1);
            }
        
            function pointInTriangle(x1, y1, x2, y2, x3, y3, x, y:Number):Boolean
            {
             var checkSide1:Boolean = side(x1, y1, x2, y2, x, y) >= 0;
             var checkSide2:Boolean = side(x2, y2, x3, y3, x, y) >= 0;
             var checkSide3:Boolean = side(x3, y3, x1, y1, x, y) >= 0;
             return checkSide1 && checkSide2 && checkSide3;
            }
            */
    }
    
};

class NavMesh {
public:
    NavMesh();
    ~NavMesh();
    
    void Build(const std::vector<Vector2>& points);
    std::vector<NavTriangle*> FindPath(NavTriangle* startTriangle, const Vector2& start, NavTriangle* endTriangle, const Vector2& end);
    NavTriangle* FindTriangle(const Vector2& position);
    std::vector<Vector2> FindStraightPath(const std::vector<NavTriangle*>& path);
private:
    bool doesListContainPoint(const std::vector<Vector2>& list, const Vector2& point);
    float triangleArea(const Vector2& a, const Vector2& b, const Vector2& c);
    typedef std::vector<NavTriangle> Triangles;
    Triangles triangles;
};