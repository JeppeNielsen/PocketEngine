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
#include <functional>
#include "GameWorld.hpp"

using namespace Pocket;

class NavTriangle;

class NavMesh {
public:
    typedef std::vector<Vector2> Vertices;
    typedef std::vector<NavTriangle*> Triangles;
    
    
    NavMesh();
    ~NavMesh();
    
    void BuildPointsTriangle(int width, int depth, std::function<bool(int x, int z)> predicate);
    std::vector<NavTriangle*> FindPath(const Vertices& vertices, NavTriangle* startTriangle, const Vector2& start, NavTriangle* endTriangle, const Vector2& end);
    std::vector<Vector2> FindStraightPath(const Vertices& vertices, const std::vector<NavTriangle*>& path);
    NavTriangle* FindNearestTriangle(const Vertices& vertices, const Vector2& position, Vector2& nearestPosition);
    const Triangles& GetTriangles() const;

    void TrimSmallTriangles();
    void Grow(Vertices& vertices, float amount);
    
    Vertices collision;
    Vertices navigation;
    
private:
    float triangleArea(const Vector2& a, const Vector2& b, const Vector2& c);
    void AddHole(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, Vector2 p, Vector2 size);
    void AddTriangle(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, Vector2& p1, Vector2& p2, Vector2& p3);
    void AddPoly(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, std::vector<Vector2>& polyPoints);
    Triangles triangles;
};


struct NavTriangle {
    short corners[3];
    NavTriangle* neighbors[3];
    
    inline Vector2 GetMidpoint(const NavMesh::Vertices& vertices, int neighborIndex) {
        if (neighborIndex<2) {
            return (vertices[corners[neighborIndex]] + vertices[corners[neighborIndex + 1]]) * 0.5f;
        } else {
            return (vertices[corners[2]] + vertices[corners[0]]) * 0.5f;
        }
    }
    
    inline int FindNeighborIndex(NavTriangle* neighbor) {
        for (int i=0; i<3; i++) {
            if (neighbors[i]==neighbor) return i;
        }
        return -1;
    }
    
    void Reset() {
        pathID = 0;
        h = 0;
        g = 0;
        total = 0;
        parent = 0;
        position = 0;
        islandID = 0;
    }
    int index;
    float h;
    float g;
    float total;
    NavTriangle* parent;
    Vector2 position;
    int pathID;
    int islandID;
    
    inline float side(const Vector2& p1, const Vector2& p2, const Vector2& p) {
        return (p2.y - p1.y) * (p.x - p1.x) + (-p2.x + p1.x)*(p.y - p1.y);
    }
    
    inline float area(const NavMesh::Vertices& vertices) {
        const Vector2& a = vertices[corners[0]];
        const Vector2& b = vertices[corners[1]];
        const Vector2& c = vertices[corners[2]];
        return (a.x * (b.y - c.y)+b.x*(c.y-a.y)+c.x*(a.y - b.y)) * 0.5f;
    }
    
    
    float GetDistance(const NavMesh::Vertices& vertices, const Vector2& w, Vector2& ret) {
    
    float ax0;    float ay0;
      float bx0;    float by0;
      float ax1;    float ay1;
      float bx1;    float by1;
      float ax2;    float ay2;
      float bx2;    float by2;
      float dz0;    float dz1;    float dz2;
      float rx;     float ry;     float rz;
      float wd;
      float d;
        
        
      // ok so the triangle has multiple zones
      // based on if we are closest to an edge, vertex or inside the triangle
      // (also if we are right on a vertex or edge)

      // by figuring out which side on each 3 sides of the triangle our point is
      // we can figure out which zone the point is in
      // e.g. if we are front of all 3 edges we are inside
      // if we are only front of one of them then we are closest to a vertex
      // if we are are front of two then we are closest to one of the edges    
      //  also because we us this the winding is important

      // if we are right on an edge or vertex then we got a special case
      // here its handled by assuming that a point on the edge is front of it
      // aka a point on a vertex is considered inside the triangle
   
        Vector2 corners[3];
        corners[0] = vertices[this->corners[0]];
        corners[1] = vertices[this->corners[1]];
        corners[2] = vertices[this->corners[2]];
        


      ax0 = corners[1].x - corners[0].x;
      ay0 = corners[1].y - corners[0].y;
      bx0 = w.x - corners[0].x;
      by0 = w.y - corners[0].y;
      dz0 = (ax0 * by0) - (ay0 * bx0);


      ax1 = corners[2].x - corners[1].x;
      ay1 = corners[2].y - corners[1].y;
      bx1 = w.x - corners[1].x;
      by1 = w.y - corners[1].y;
      dz1 = (ax1 * by1) - (ay1 * bx1);


      ax2 = corners[0].x - corners[2].x;
      ay2 = corners[0].y - corners[2].y;
      bx2 = w.x - corners[2].x;
      by2 = w.y - corners[2].y;
      dz2 = (ax2 * by2) - (ay2 * bx2);

      //+++ //inside
      if (dz0 >= 0 && dz1 >= 0 && dz2 >=0) 
      {
        d = 0;
        ret.x = w.x; ret.y = w.y;
      }
      else   //-+- //vertex
      if (dz0 < 0 && dz1 >= 0 && dz2 < 0) 
      { 
        d = bx0*bx0 + by0*by0;
        ret = corners[0];
      }
      else   //--+ //vertex
      if (dz0 < 0 && dz1 < 0 && dz2 >= 0) 
      {
        d = bx1*bx1 + by1*by1;
        ret = corners[1];
      }
      else   //+-- //vertex
      if (dz0 >= 0 && dz1 < 0 && dz2 < 0)
      { 
        d = bx2*bx2 + by2*by2;
        ret = corners[2];
      }
      else //-++  //edge
      if (dz0 < 0 && dz1 >= 0 && dz2 >= 0) 
      {
        wd = ((ax0 * bx0) + (ay0 * by0) ) / ((ax0 * ax0) + (ay0 * ay0));
        if (wd < 0) { wd = 0;} if (wd > 1) { wd = 1; }
        rx = corners[0].x + (corners[1].x - corners[0].x) * wd;
        ry = corners[0].y + (corners[1].y - corners[0].y) * wd;

        ret.x = rx; ret.y = ry;
        rx = w.x-rx;   ry = w.y-ry;
        d = rx*rx + ry*ry;
      }
      else  //+-+ //edge
      if (dz0 >= 0 && dz1 < 0 && dz2 >= 0) 
      {
        wd = ((ax1 * bx1) + (ay1 * by1) ) / ((ax1 * ax1) + (ay1 * ay1));
        if (wd < 0) { wd = 0;} if (wd > 1) { wd = 1; }
        rx = corners[1].x + (corners[2].x - corners[1].x) * wd;
        ry = corners[1].y + (corners[2].y - corners[1].y) * wd;
        
        ret.x = rx; ret.y = ry;
        rx = w.x-rx;   ry = w.y-ry;
        d = rx*rx + ry*ry;
      }
      else   //++- //edge
      if (dz0 >= 0 && dz1 >= 0 && dz2 < 0) 
      {
        wd = ((ax2 * bx2) + (ay2 * by2) ) / ((ax2 * ax2) + (ay2 * ay2));
        if (wd < 0) { wd = 0;} if (wd > 1) { wd = 1; }
        rx = corners[2].x + (corners[0].x - corners[2].x) * wd;
        ry = corners[2].y + (corners[0].y - corners[2].y) * wd;

        ret.x = rx; ret.y = ry;
        rx = w.x-rx;   ry = w.y-ry;
        d = rx*rx + ry*ry;
      }
 
      return d;
    }
    
    
        /* Check whether P and Q lie on the same side of line AB */
    inline float Side(const Vector2& p, const Vector2& q, const Vector2& a, const Vector2& b) {
        float z1 = (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
        float z2 = (b.x - a.x) * (q.y - a.y) - (q.x - a.x) * (b.y - a.y);
        return z1 * z2;
    }

    /* Check whether segment P0P1 intersects with triangle corners[0]corners[1]corners[2] */
    bool SegmentTouching(const NavMesh::Vertices& vertices, const Vector2& p0, const Vector2& p1)
    {
        Vector2 corners[3];
        corners[0] = vertices[this->corners[0]];
        corners[1] = vertices[this->corners[1]];
        corners[2] = vertices[this->corners[2]];
    
        /* Check whether segment is outside one of the three half-planes
         * delimited by the triangle. */
        float f1 = Side(p0, corners[2], corners[0], corners[1]), f2 = Side(p1, corners[2], corners[0], corners[1]);
        float f3 = Side(p0, corners[0], corners[1], corners[2]), f4 = Side(p1, corners[0], corners[1], corners[2]);
        float f5 = Side(p0, corners[1], corners[2], corners[0]), f6 = Side(p1, corners[1], corners[2], corners[0]);
        /* Check whether triangle is totally inside one of the two half-planes
         * delimited by the segment. */
        float f7 = Side(corners[0], corners[1], p0, p1);
        float f8 = Side(corners[1], corners[2], p0, p1);

        /* If segment is strictly outside triangle, or triangle is strictly
         * apart from the line, we're not intersecting */
        if ((f1 < 0 && f2 < 0) || (f3 < 0 && f4 < 0) || (f5 < 0 && f6 < 0)
              || (f7 > 0 && f8 > 0))
            return false;//NOT_INTERSECTING;

        /* If segment is aligned with one of the edges, we're overlapping */
        if ((f1 == 0 && f2 == 0) || (f3 == 0 && f4 == 0) || (f5 == 0 && f6 == 0))
            return true;//OVERLAPPING;

        /* If segment is outside but not strictly, or triangle is apart but
         * not strictly, we're touching */
        if ((f1 <= 0 && f2 <= 0) || (f3 <= 0 && f4 <= 0) || (f5 <= 0 && f6 <= 0)
              || (f7 >= 0 && f8 >= 0))
            return true;// TOUCHING;

        /* If both segment points are strictly inside the triangle, we
         * are not intersecting either */
        if (f1 > 0 && f2 > 0 && f3 > 0 && f4 > 0 && f5 > 0 && f6 > 0)
            return true;//NOT_INTERSECTING;

        /* Otherwise we're intersecting with at least one edge */
        return true;//INTERSECTING;
    }
};
