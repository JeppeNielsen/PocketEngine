//
//  NavMesh.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 17/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "NavMesh.hpp"
#include <assert.h>

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
    
    if (startTriangle == endTriangle) {
        path.push_back(startTriangle);
        return path;
    }
    
    std::vector<NavTriangle*> openList;
    startTriangle->position = start;
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
























