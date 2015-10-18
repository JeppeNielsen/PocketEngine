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
    
    
    //path is reversed
    
    portalsLeft.push_back(path[path.size()-1]->position);
    portalsRight.push_back(path[path.size()-1]->position);
    
    
    for (int i=(int)path.size()-1; i>=1; i--) {
        NavTriangle* current = path[i];
        NavTriangle* next = path[i-1];
        int forwardIndex = current->FindNeighborIndex(next);
        
        portalsRight.push_back(current->corners[forwardIndex]);
        portalsLeft.push_back(forwardIndex<2 ? current->corners[forwardIndex + 1] : current->corners[0]);
        /*
        if (!doesListContainPoint(leftSides, current->corners[forwardIndex])) {
            leftSides.push_back();
        }
        
        if (forwardIndex < 2) {
            if (!doesListContainPoint(rightSides, current->corners[forwardIndex + 1])) {
                rightSides.push_back(current->corners[forwardIndex + 1]);
            }
        } else {
            if (!doesListContainPoint(rightSides, current->corners[0])) {
                rightSides.push_back(current->corners[0]);
            }
        }
        */
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

bool NavMesh::doesListContainPoint(const std::vector<Vector2> &list, const Pocket::Vector2 &point) {
    for(auto& p : list) {
        if (p==point) return true;
    }
    return false;
}




















