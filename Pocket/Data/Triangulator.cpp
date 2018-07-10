//
//  Triangulator.cpp
//  MotoCross
//
//  Created by Jeppe Nielsen on 2/9/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Triangulator.hpp"

using namespace Pocket;

static const float EPSILON=0.0000000001f;

float Triangulator::Area(const Vertices& vertices)
{
    
    size_t n = vertices.size();
    
    float A=0.0f;
    
    for(size_t p=n-1,q=0; q<n; p=q++)
    {
        A+= vertices[p].x*vertices[q].y - vertices[q].x*vertices[p].y;
    }
    return A*0.5f;
}

/*
 InsideTriangle decides if a point P is Inside of the triangle
 defined by A, B, C.
 */
bool Triangulator::InsideTriangle(float Ax, float Ay,
                                 float Bx, float By,
                                 float Cx, float Cy,
                                 float Px, float Py)

{
    float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    float cCROSSap, bCROSScp, aCROSSbp;
    
    ax = Cx - Bx;  ay = Cy - By;
    bx = Ax - Cx;  by = Ay - Cy;
    cx = Bx - Ax;  cy = By - Ay;
    apx= Px - Ax;  apy= Py - Ay;
    bpx= Px - Bx;  bpy= Py - By;
    cpx= Px - Cx;  cpy= Py - Cy;
    
    aCROSSbp = ax*bpy - ay*bpx;
    cCROSSap = cx*apy - cy*apx;
    bCROSScp = bx*cpy - by*cpx;
    
    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool Triangulator::Snip(const Vertices& vertices,int u,int v,int w,int n,int *V)
{
    int p;
    float Ax, Ay, Bx, By, Cx, Cy, Px, Py;
    
    Ax = vertices[V[u]].x;
    Ay = vertices[V[u]].y;
    
    Bx = vertices[V[v]].x;
    By = vertices[V[v]].y;
    
    Cx = vertices[V[w]].x;
    Cy = vertices[V[w]].y;
    
    if ( EPSILON > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax))) ) return false;
    
    for (p=0;p<n;p++)
    {
        if( (p == u) || (p == v) || (p == w) ) continue;
        Px = vertices[V[p]].x;
        Py = vertices[V[p]].y;
        if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) return false;
    }
    
    return true;
}

bool Triangulator::Triangulate(const Vertices& vertices, IndiciesVector &indicies)
{
    /* allocate and initialize list of Vertices in polygon */
    
    int n = (int)vertices.size();
    if ( n < 3 ) return false;
    
    int *V = new int[n];
    
    /* we want a counter-clockwise polygon in V */
    
    if ( 0.0f < Area(vertices) )
        for (int v=0; v<n; v++) V[v] = v;
    else
        for(int v=0; v<n; v++) V[v] = (n-1)-v;
    
    int nv = n;
    
    /*  remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2*nv;   /* error detection */
    
    for(int m=0, v=nv-1; nv>2; )
    {
        /* if we loop, it is probably a non-simple polygon */
        if (0 >= (count--))
        {
            //** Triangulate: ERROR - probable bad polygon!
            return false;
        }
        
        /* three consecutive vertices in current polygon, <u,v,w> */
        int u = v  ; if (nv <= u) u = 0;     /* previous */
        v = u+1; if (nv <= v) v = 0;     /* new v    */
        int w = v+1; if (nv <= w) w = 0;     /* next     */
        
        if ( Snip(vertices,u,v,w,nv,V) )
        {
            int a,b,c,s,t;
            
            /* true names of the vertices */
            a = V[u]; b = V[v]; c = V[w];
            
            /* output Triangle */
            indicies.push_back( a );
            indicies.push_back( b );
            indicies.push_back( c );
            
            m++;
            
            /* remove v from remaining polygon */
            for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t]; nv--;
            
            /* resest error detection counter */
            count = 2*nv;
        }
    }
    
    delete[] V;
    return true;
}
