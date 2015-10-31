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
#include "Transform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Triangulator.hpp"
#include "clipper.hpp"

NavMesh::NavMesh() {}
NavMesh::~NavMesh() { ClearTriangles(); }

void NavMesh::ClearTriangles() {
    for (int i=0; i<triangles.size(); i++) {
        delete triangles[i];
    }
    triangles.clear();
    trianglesPerVertex.clear();
}

int findCornerIndexFromNeighborIndex(int corner1, int corner2) {
    if (corner1 == 0) {
        return corner2 == 1 ? 0 : 2;
    } else if (corner1 == 1) {
        return corner2 == 0 ? 0 : 1;
    } else {
        return corner2 == 1 ? 1 : 2;
    }
}

void NavMesh::AddTriangle(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, Vector2& p1, Vector2& p2, Vector2& p3) {
    int index = (int)points.size()/2;
    points.push_back(p1.x);
    points.push_back(p1.y);

    points.push_back(p2.x);
    points.push_back(p2.y);

    points.push_back(p3.x);
    points.push_back(p3.y);

    segments.push_back(index+0);
    segments.push_back(index+1);

    segments.push_back(index+1);
    segments.push_back(index+2);

    segments.push_back(index+2);
    segments.push_back(index+0);
}

void NavMesh::AddPoly(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, std::vector<Vector2>& polyPoints) {
    int index = (int)points.size()/2;
    
    Vector2 holePosition = 0;
    for (int i=0; i<polyPoints.size(); i++) {
        points.push_back(polyPoints[i].x);
        points.push_back(polyPoints[i].y);
    
        segments.push_back(index + i);
        segments.push_back(index + ((i==polyPoints.size()-1) ? 0 : i + 1));
        
        holePosition += polyPoints[i];
        
    }
    holePosition *= (1.0f / polyPoints.size());
    
     for (int i=0; i<polyPoints.size(); i++) {
        Vector2 toCenter = holePosition - polyPoints[i];
        toCenter.Normalize();
        holes.push_back(polyPoints[i].x + toCenter.x * 4);
        holes.push_back(polyPoints[i].y + toCenter.y * 4);
    }
    
}


void NavMesh::AddHole(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, Vector2 p, Vector2 size) {
    int index = (int)points.size()/2;
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
    for (int i=0; i<triangles.size(); i++) {
        delete triangles[i];
    }
    triangles.clear();
    navigation.clear();
    collision.clear();
    trianglesPerVertex.clear();
    
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

    
    int lastPos = width - 1;
    for (int z=0; z<depth; ++z) {
        int unblockPos = 0;
        for (int x=0; x<width; ++x) {
            bool blocked = predicate(x,z);
            //if (blocked) {
            //    AddHole(outer, segments, holes, {(float)x,(float)z}, {1.0f, 1.0f});
            //}
            //continue;
            
            if (!blocked || x == lastPos) {
                int size = x - unblockPos;
                if (x == lastPos ) {
                    size++;
                }
                if (size>0) {
                    AddHole(outer, segments, holes, {(float)unblockPos,(float)z}, {(float)size, 1.0f});
                }
                unblockPos = x + 1;
            }
        }
    }
    
    triangulateio in;
    memset(&in, 0, sizeof(triangulateio));
    in.numberofpoints = (int)outer.size()/2;
    in.pointlist = &outer[0];
    
    in.holelist = &holes[0];
    in.numberofholes = (int)holes.size()/2;
    
    in.segmentlist = &segments[0];
    in.numberofsegments = (int)segments.size()/2;
    
    triangulateio out;
    memset(&out, 0, sizeof(triangulateio));
    
    triangulate("zpnQ", &in, &out, (struct triangulateio *) NULL );
    
    triangles.resize(out.numberoftriangles);
    for (int i=0; i<out.numberoftriangles; i++) {
        triangles[i] = new NavTriangle();
    }
    
    collision.resize(out.numberofpoints);
    trianglesPerVertex.resize(out.numberofpoints);
    
    for (int i=0; i<out.numberofpoints; i++) {
        double* pos = &out.pointlist[i*2];
        collision[i].x = (float)pos[0];
        collision[i].y = (float)pos[1];
    }
    navigation = collision;
    
   
    
    std::cout<<"-------------"<<std::endl;
    for (int i=0; i<out.numberoftriangles; i++) {
        int* triangleIndex = &out.trianglelist[i*3];
        //std::cout<<triangle[0]<<", "<<triangle[1]<<", " <<triangle[2]<<std::endl;
        NavTriangle& tri = *triangles[i];
        tri.corners[0]= triangleIndex[0];
        tri.corners[1]= triangleIndex[1];
        tri.corners[2]= triangleIndex[2];
        
        int* neighbor = &out.neighborlist[i*3];
        
        tri.neighbors[1] = neighbor[0]>=0 ? triangles[neighbor[0]] : 0;
        tri.neighbors[2] = neighbor[1]>=0 ? triangles[neighbor[1]] : 0;
        tri.neighbors[0] = neighbor[2]>=0 ? triangles[neighbor[2]] : 0;
        
        
       // std::cout<<"neightbor"<<std::endl;
       // std::cout<<neighbor[0]<<", "<<neighbor[1]<<", " <<neighbor[2]<<std::endl;
    }
    
    trifree(out.pointlist);
    trifree(out.neighborlist);
    trifree(out.trianglelist);
    
    TrimSmallTriangles();
    Grow(navigation, -0.5f);
    
    version++;
}

std::vector<NavTriangle*> NavMesh::FindPath(const Vertices& vertices, NavTriangle* startTriangle, const Pocket::Vector2 &start, NavTriangle* endTriangle, const Pocket::Vector2 &end) {
    
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
            
            Vector2 midPoint = current->GetMidpoint(vertices, n);
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

std::vector<Vector2> NavMesh::FindStraightPath(const Vertices& vertices, const std::vector<NavTriangle *> &path) {
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
        
        portalsRight.push_back(vertices[current->corners[forwardIndex]]);
        portalsLeft.push_back(forwardIndex<2 ? vertices[current->corners[forwardIndex + 1]] : vertices[current->corners[0]]);
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
    
    /*
    const float radius = 1.0f;
    
    std::vector<Vector2> offsetedPath;
    offsetedPath.push_back(straightPath[0]);
    
    for (int i=1; i<straightPath.size()-1; i++) {
        Vector2 forward = straightPath[i] - straightPath[i-1];
        Vector2 backwards = straightPath[i] - straightPath[i+1];
        forward.Normalize();
        backwards.Normalize();
        Vector2 normal = forward + backwards;
        normal.Normalize();
        offsetedPath.push_back(straightPath[i]+normal * radius);
    }
    offsetedPath.push_back(straightPath[straightPath.size() - 1]);
    */
    return straightPath;
}

float NavMesh::triangleArea(const Vector2& a, const Vector2& b, const Vector2& c) {
    const Vector2 ab = b-a;
    const Vector2 ac = c-a;
    return ac.x*ab.y - ab.x*ac.y;
}

NavTriangle* NavMesh::FindNearestTriangle(const Vertices& vertices, const Pocket::Vector2 &position, Vector2& nearestPosition, int& navMeshVersion, NavTriangle* hintTriangle) {

    if (navMeshVersion!=version) {
        hintTriangle = 0;
        navMeshVersion = version;
    }

    if (triangles.empty()) {
        nearestPosition = position;
        return 0;
    }

    float minDistance = 1000000.0f;
    Vector2 ret;
    NavTriangle* foundTriangle = 0;
    if (!hintTriangle) {
        for (int i=0; i<triangles.size(); i++) {
            float distance = triangles[i]->GetDistance(vertices, position, ret);
            if (distance<minDistance) {
                minDistance = distance;
                nearestPosition = ret;
                foundTriangle = triangles[i];
                if (minDistance<0.001f) return foundTriangle;
            }
        }
    } else {
        for (int n=0; n<3; n++) {
            Triangles& triangles = trianglesPerVertex[hintTriangle->corners[n]];
            for (int i=0; i<triangles.size(); i++) {
                float distance = triangles[i]->GetDistance(vertices, position, ret);
                if (distance<minDistance) {
                    minDistance = distance;
                    nearestPosition = ret;
                    foundTriangle = triangles[i];
                    if (minDistance<0.001f) return foundTriangle;
                }
            }
        }
    }
    return foundTriangle;
}

const NavMesh::Triangles& NavMesh::GetTriangles() const { return triangles; }

void NavMesh::TrimSmallTriangles() {
    
    for (int i=0; i<triangles.size(); i++) {
        NavTriangle* t = triangles[i];
        int numEdges = 0;
        for (int n=0; n<3; n++) {
            if (!t->neighbors[n]) numEdges++;
        }
        
        if (numEdges>=2) {
            for (int n=0; n<3; n++) {
                NavTriangle* neighbor = t->neighbors[n];
                if (!neighbor) continue;
                int neighborIndex = neighbor->FindNeighborIndex(t);
                neighbor->neighbors[neighborIndex] = 0;
                break;
            }
            
            triangles.erase(triangles.begin()+i);
            i--;
        }
    }
    
     for (int i=0; i<triangles.size(); i++) {
        NavTriangle* t = triangles[i];
        int numEdges = 0;
        for (int n=0; n<3; n++) {
            if (!t->neighbors[n]) numEdges++;
        }
        if (numEdges==3) {
            triangles.erase(triangles.begin()+i);
            i--;
        }
    }
    
    for (int i=0; i<triangles.size(); i++) {
        NavTriangle* triangle = triangles[i];
        trianglesPerVertex[triangle->corners[0]].push_back(triangle);
        trianglesPerVertex[triangle->corners[1]].push_back(triangle);
        trianglesPerVertex[triangle->corners[2]].push_back(triangle);
    }
}

void NavMesh::Grow(Vertices& vertices, float amount) {
    
    std::vector<Vector2> normals;
    normals.resize(vertices.size(), Vector2(0,0));
    for(NavTriangle* t : triangles) {
        for (int i=0; i<3; i++) {
            if (!t->neighbors[i]) {
                int index0 = t->corners[i];
                int index1 = t->corners[i<2 ? i + 1 : 0];
                Vector2 direction = vertices[index1] - vertices[index0];
                Vector2 normal = { -direction.y, direction.x };
                normal.Normalize();
                normals[index0] += normal;
                normals[index1] += normal;
            }
        }
    }

    for (int i=0; i<vertices.size(); i++) {
        vertices[i] -= normals[i].Normalized() * amount;
    }
}