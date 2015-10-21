//
//  NavMesh.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 17/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "NavMesh.hpp"
#include <assert.h>
#include "triangle.h"

NavMesh::NavMesh() {}
NavMesh::~NavMesh() {}

int findCornerIndexFromNeighborIndex(int corner1, int corner2) {
    if (corner1 == 0) {
        return corner2 == 1 ? 0 : 2;
    } else if (corner1 == 1) {
        return corner2 == 0 ? 0 : 1;
    } else {
        return corner2 == 1 ? 1 : 2;
    }
}

void NavMesh::AddHole(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, Vector2 p, Vector2 size) {
      int index = points.size()/2;
        points.push_back(p.x);
        points.push_back(p.y);
        
        points.push_back(p.x+size.x);
        points.push_back(p.y);
        
        points.push_back(p.x);
        points.push_back(p.y+size.y);
        
        points.push_back(p.x+size.x);
        points.push_back(p.y+size.y);
        
        segments.push_back(index+0);
        segments.push_back(index+1);
        
        segments.push_back(index+1);
        segments.push_back(index+3);
        
        segments.push_back(index+3);
        segments.push_back(index+2);
        
        segments.push_back(index+2);
        segments.push_back(index+0);
        
        holes.push_back(p.x + size.x * 0.5f);
        holes.push_back(p.y + size.y * 0.5f);
}

void NavMesh::BuildPointsTriangle(int width, int depth, std::function<bool (int, int)> predicate) {
    
    std::vector<double> outer = {
        0,0,
        (double)width,0,
        0,(double)depth,
        (double)width,(double)depth,
        };
    
    std::vector<int> segments = {
        0,1,
        1,3,
        3,2,
        2,0,
    };
    
    std::vector<double> holes;

    for (int z=0; z<depth; ++z) {
        for (int x=0; x<width; ++x) {
            if (predicate(x,z)) {
                AddHole(outer, segments, holes, {(float)x,(float)z}, 1.0f);
            }
        }
    }

    triangulateio in;
    memset(&in, 0, sizeof(triangulateio));
    in.numberofpoints = outer.size()/2;
    in.pointlist = &outer[0];
    
    in.holelist = &holes[0];
    in.numberofholes = holes.size()/2;
    
    in.segmentlist = &segments[0];
    //in.segmentmarkerlist = &segmentMarker[0];
    in.numberofsegments = segments.size()/2;
    
    triangulateio out;
    memset(&out, 0, sizeof(triangulateio));
    
    triangulate("zpnQ", &in, &out, NULL );
    
    for (int i=0; i<out.numberofpoints; i++) {
        double* p = &out.pointlist[i*2];
       // std::cout<<p[0]<<","<<p[1]<<std::endl;
    }
    
    triangles.resize(out.numberoftriangles);
    
    std::cout<<"-------------"<<std::endl;
    for (int i=0; i<out.numberoftriangles; i++) {
        int* triangleIndex = &out.trianglelist[i*3];
        //std::cout<<triangle[0]<<", "<<triangle[1]<<", " <<triangle[2]<<std::endl;
        
        double* pos1 = &out.pointlist[triangleIndex[0]*2];
        double* pos2 = &out.pointlist[triangleIndex[1]*2];
        double* pos3 = &out.pointlist[triangleIndex[2]*2];
        
        NavTriangle& tri = triangles[i];
        tri.corners[0]= { (float)pos1[0], (float)pos1[1]};
        tri.corners[1]= { (float)pos2[0], (float)pos2[1]};
        tri.corners[2]= { (float)pos3[0], (float)pos3[1]};
        
        
        int* neighbor = &out.neighborlist[i*3];
        
        tri.neighbors[1] = neighbor[0]>=0 ? &triangles[neighbor[0]] : 0;
        tri.neighbors[2] = neighbor[1]>=0 ? &triangles[neighbor[1]] : 0;
        tri.neighbors[0] = neighbor[2]>=0 ? &triangles[neighbor[2]] : 0;
        
        
       // std::cout<<"neightbor"<<std::endl;
       // std::cout<<neighbor[0]<<", "<<neighbor[1]<<", " <<neighbor[2]<<std::endl;
    }
}

/*
std::vector<Vector2> NavMesh::BuildPoints(int width, int depth, std::function<bool (int, int)> predicate) {
    
    const int Scaling = 100;
    
    Clipper clipper;
    ClipperLib::Path outer;
    outer.push_back({0, 0});
    outer.push_back({0, depth * Scaling});
    outer.push_back({width * Scaling, depth * Scaling});
    outer.push_back({width * Scaling, 0});
    
    clipper.AddPath(outer, ClipperLib::ptSubject, true);
    
    ClipperLib::Path obstruction;
    obstruction.resize(4);
    
    for (int z=0; z<depth; ++z) {
        for (int x=0; x<width; ++x) {
            if (predicate(x,z)) {
                int xx = x * Scaling;
                int zz = z * Scaling;
                
                obstruction[0].X = xx;
                obstruction[0].Y = zz;
                
                obstruction[1].X = xx;
                obstruction[1].Y = zz + Scaling;
                
                obstruction[2].X = xx + Scaling;
                obstruction[2].Y = zz + Scaling;
                
                obstruction[3].X = xx + Scaling;
                obstruction[3].Y = zz;
                
                clipper.AddPath(obstruction, ClipperLib::ptClip, true);
            }
        }
    }
    
    ClipperLib::Paths solution;
    clipper.Execute(ClipperLib::ctDifference, solution);
    
    std::vector<Vector2> points;
    TesselateSolution(solution, points);
    return points;
}

void NavMesh::TesselateSolution(const ClipperLib::Paths &solution, std::vector<Vector2> &points) {
    
    const float InvScaling = 1.0f / 100.0f;
    
    TESStesselator* tess = tessNewTess(0);
    
    for (const ClipperLib::Path& path : solution) {
        std::vector<float> contour(path.size()*2);
        for (int i=0; i<path.size(); ++i) {
            contour[i*2] = path[i].X * InvScaling;
            contour[i*2 + 1] = path[i].Y * InvScaling;
        }
        tessAddContour(tess, 2, &contour[0], sizeof(float)*2, (int)path.size());
    }
    
    int polySize = 3;
    //int error = tessTesselate(tess, TESS_WINDING_NONZERO, polySize, 3, 2, NULL);
    // ( TESStesselator *tess, int windingRule, int elementType, int polySize, int vertexSize, const TESSreal* normal );

    int succes = tessTesselate(tess, TESS_WINDING_NONZERO, TESS_CONNECTED_POLYGONS, polySize, 2, NULL);

    if (succes) {
        const TESSreal* verts = tessGetVertices( tess );
        const int nelems = tessGetElementCount(tess);
        const TESSindex* elems = tessGetElements(tess);
     
        for (int i = 0; i < nelems; i++) {
            const TESSindex* poly = &elems[i * polySize*2];
            NavTriangle triangle;
            
            for (int j = 0; j < polySize; j++) {
                if (poly[j] == TESS_UNDEF) break;
                const TESSreal* pos = &verts[poly[j]*2];
                points.push_back({pos[0], pos[1]});
                triangle.corners[j] = { pos[0], pos[1] };
            }
            triangle.Reset();
            triangles.push_back(triangle);
        }
        
        for (int i = 0; i < nelems; i++) {
            const TESSindex* poly = &elems[i * polySize*2];
            NavTriangle& triangle = triangles[i];
            const TESSindex* nei = &poly[polySize];
            for (int j=0; j<polySize; j++) {
                triangle.neighbors[j]= nei[j]==-1 ? 0 : &triangles[nei[j]];
            }
        }
    }
    
    tessDeleteTess(tess);
}
*/
void NavMesh::Build(const std::vector<Vector2>& points) {
    assert(points.size() % 3 == 0);//, "Points must be multiply of three");
    int numTriangles = (int)points.size() / 3;
    triangles.resize(numTriangles);
    
    for (int i=0; i<numTriangles; i++) {
        NavTriangle& triangle = triangles[i];
        triangle.neighbors[0]=0;
        triangle.neighbors[1]=0;
        triangle.neighbors[2]=0;
        triangle.Reset();
        triangle.index = i;
        for (int c=0; c<3; c++) {
            triangle.corners[c] = points[i * 3 + c];
        }
    
    /*
    //find neighbors
    for (int i=0; i<numTriangles; i++) {
        NavTriangle& current = triangles[i];
        for (int j=0; j<numTriangles; j++) {
            if (j==i) continue;
            NavTriangle& other = triangles[j];
            int dummy;
            if (other.hasCorner(current.corners[0], dummy) && other.hasCorner(current.corners[1], dummy)){
                current.neighbors[0]=&other;
                continue;
            }
            if (other.hasCorner(current.corners[1], dummy) && other.hasCorner(current.corners[2], dummy)){
                current.neighbors[1]=&other;
                continue;
            }
            if (other.hasCorner(current.corners[2], dummy) && other.hasCorner(current.corners[0], dummy)){
                current.neighbors[2]=&other;
                continue;
            }
        }
    }
    */
 
        //check for neighbors
        for (int j=0; j<i; j++) {
            NavTriangle& other = triangles[j];
            int corner1;
            int corner2;
            
            if (other.hasCorner(triangle.corners[0], corner1) && other.hasCorner(triangle.corners[1], corner2)) {
                triangle.neighbors[0] = &other;
                other.neighbors[findCornerIndexFromNeighborIndex(corner1, corner2)]=&triangle;
                continue;
            }
            
            if (other.hasCorner(triangle.corners[1], corner1) && other.hasCorner(triangle.corners[2], corner2)) {
                triangle.neighbors[1] = &other;
                other.neighbors[findCornerIndexFromNeighborIndex(corner1, corner2)]=&triangle;
                continue;
            }
            
             if (other.hasCorner(triangle.corners[2], corner1) && other.hasCorner(triangle.corners[0], corner2)) {
                triangle.neighbors[2] = &other;
                other.neighbors[findCornerIndexFromNeighborIndex(corner1, corner2)]=&triangle;
                continue;
            }
        }
    }
}

std::vector<NavTriangle*> NavMesh::FindPath(NavTriangle* startTriangle, const Pocket::Vector2 &start, NavTriangle* endTriangle, const Pocket::Vector2 &end) {
    
    std::vector<NavTriangle*> path;
    
    std::vector<NavTriangle*> openList;
    startTriangle->position = start;
    startTriangle->parent = 0;
    openList.push_back(startTriangle);
    
    NavTriangle* targetNode = 0;
    
    static int pathID = 1;
    
    int inClosedListID = pathID + 1;
    
    while (true) {
    
        if (openList.empty()) {
            break;
        }
    
        std::sort(openList.begin(), openList.end(), [] (const NavTriangle* a, NavTriangle* b) {
            return a->total>b->total;
        });
        
        NavTriangle* current = openList.back();
        openList.pop_back();
        current->pathID = inClosedListID;
        
        if (current == endTriangle) {
            current->position = end;
            targetNode = current;
            break; // end found
        }
        
        for (int n=0; n<3; n++) {
            NavTriangle* neighbor = current->neighbors[n];
            if (!neighbor) continue;
            if (neighbor->pathID == inClosedListID) continue; // in closed list
            
            Vector2 midPoint = current->GetMidpoint(n);
            Vector2 toEnd = end - midPoint;
            Vector2 toParent = current->position - midPoint;
            
            if (neighbor->pathID<pathID) {
                neighbor->h = toEnd.Length();
                neighbor->g = current->g + toParent.Length();
                neighbor->total = neighbor->h + neighbor->g;
                neighbor->parent = current;
                neighbor->position = midPoint;
                neighbor->pathID = pathID;
                openList.push_back(neighbor);
            } else {
                float tempCost = current->g + toParent.Length();
                if (tempCost<neighbor->g) {
                    neighbor->parent = current;
                    neighbor->g = tempCost;
                    neighbor->total = neighbor->h + neighbor->g;
                }
            }
        }
    }
    
    while (targetNode) {
        path.push_back(targetNode);
        targetNode = targetNode->parent;
    }
    
    pathID+=2;
    return path;
}

NavTriangle* NavMesh::FindTriangle(const Pocket::Vector2 &position) {
    for (NavTriangle& triangle : triangles) {
        if (triangle.PointInside(position)) {
            return &triangle;
        }
    }
    return 0;
}

std::vector<Vector2> NavMesh::FindStraightPath(const std::vector<NavTriangle *> &path) {
    std::vector<Vector2> straightPath;
    
    if (path.empty()) {
        return straightPath;
    }
    
    std::vector<Vector2> portalsLeft;
    std::vector<Vector2> portalsRight;
    
    portalsLeft.push_back(path[path.size()-1]->position);
    portalsRight.push_back(path[path.size()-1]->position);
    
    for (int i=(int)path.size()-1; i>=1; i--) {
        NavTriangle* current = path[i];
        NavTriangle* next = path[i-1];
        int forwardIndex = current->FindNeighborIndex(next);
        
        portalsRight.push_back(current->corners[forwardIndex]);
        portalsLeft.push_back(forwardIndex<2 ? current->corners[forwardIndex + 1] : current->corners[0]);
    }
    
    portalsLeft.push_back(path[0]->position);
    portalsRight.push_back(path[0]->position);
    
    
    Vector2 portalApex = portalsLeft[0];
    Vector2 portalLeft = portalsLeft[0];
    Vector2 portalRight = portalsRight[0];
    int apexIndex = 0, leftIndex = 0, rightIndex = 0;
    
    straightPath.push_back(portalApex);
    
    for (int i=1; i<portalsLeft.size(); i++) {
        Vector2 left = portalsLeft[i];
        Vector2 right = portalsRight[i];
    
         // Update right vertex.
        if (triangleArea(portalApex, portalRight, right) <= 0.0f)
        {
            if (portalApex.EqualEpsilon(portalRight) || triangleArea(portalApex, portalLeft, right) > 0.0f) {
                // Tighten the funnel.
                portalRight = right;
                rightIndex = i;
            }
            else
            {
                // Right over left, insert left to path and restart scan from portal left point.
                 straightPath.push_back(portalLeft);
                 
                 // Make current left the new apex.
                 portalApex = portalLeft;
                 apexIndex = leftIndex;
                 // Reset portal
                 portalLeft = portalApex;
                 portalRight = portalApex;
                 leftIndex = apexIndex;
                 rightIndex = apexIndex;
                 // Restart scan
                 i = apexIndex;
                 continue;
            }
        }

        // Update left vertex.
        if (triangleArea(portalApex, portalLeft, left) >= 0.0f)
        {
             if (portalApex.EqualEpsilon(portalLeft) || triangleArea(portalApex, portalRight, left) < 0.0f)
             {
                 portalLeft = left;
                 // Tighten the funnel.
                 leftIndex = i;
             }
             else
             {
                  // Left over right, insert right to path and restart scan from portal right point.
                 straightPath.push_back(portalRight);
                 
                 // Make current right the new apex.
                 portalApex = portalRight;
                 apexIndex = rightIndex;
                     // Reset portal
                 portalLeft = portalApex;
                 portalRight = portalApex;
                 leftIndex = apexIndex;
                 rightIndex = apexIndex;
                 // Restart scan
                 i = apexIndex;
                 continue;
             }
         }
    }
    straightPath.push_back(path[0]->position);
    return straightPath;
}

float NavMesh::triangleArea(const Vector2& a, const Vector2& b, const Vector2& c) {
    const Vector2 ab = b-a;
    const Vector2 ac = c-a;
    return ac.x*ab.y - ab.x*ac.y;
}

NavTriangle* NavMesh::FindNearestTriangle(const Pocket::Vector2 &position, Vector2& nearestPosition) {
    float minDistance = 1000000.0f;
    Vector2 ret;
    NavTriangle* foundTriangle = 0;
    for (int i=0; i<triangles.size(); i++) {
        float distance = triangles[i].GetDistance(position, ret);
        if (distance<minDistance) {
            minDistance = distance;
            nearestPosition = ret;
            foundTriangle = &triangles[i];
        }
    }
    return foundTriangle;
}



std::vector<Vector2> NavMesh::Cut(const std::vector<Vector2> &points) {
    std::vector<Vector2> cut;
    for(NavTriangle& triangle : triangles) {
        //triangle.Cut(points, cut);
    }
    return cut;
}

const NavMesh::Triangles& NavMesh::GetTriangles() const { return triangles; }












