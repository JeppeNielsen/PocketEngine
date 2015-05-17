//
//  Triangulator.hpp
//  MotoCross
//
//  Created by Jeppe Nielsen on 2/9/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Vector2.hpp"

//triangulator courtesy of John W. Ratcliff [jratcliff@verant.com]
//http://www.flipcode.com/archives/triangulate.cpp

namespace Pocket {
    class Triangulator
    {
    public:
        
        typedef std::vector< Vector2 > Vertices;
        typedef std::vector< size_t > IndiciesVector;
        
        static bool Triangulate(const Vertices &vertices, IndiciesVector& indicies);
        
    private:
        static float Area(const Vertices &vertices);
        static bool InsideTriangle(float Ax, float Ay,
                                   float Bx, float By,
                                   float Cx, float Cy,
                                   float Px, float Py);
    
        static bool Snip(const Vertices &vertices,int u,int v,int w,int n,int *V);
        
    };
}