//
//  Map.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Map.h"
#include "MathHelper.hpp"
#include "clipper.hpp"
#include "clip2tri.h"

void Map::CreateMap(int width, int depth) {
	nodes.resize(width);
    for	(int i=0; i<width; i++) {
    	nodes[i].resize(depth);
    }
    outOfBoundsNode.color = Colour::White();
    outOfBoundsNode.normal = {0,1,0};
}

int Map::Width() {
	return (int)nodes.size();
}

int Map::Depth() {
	return (int)nodes[0].size();
}

Map::Node& Map::GetNode(int x, int z) {
	if (x<0) return outOfBoundsNode;
    if (z<0) return outOfBoundsNode;
    if (x>=Width()) return outOfBoundsNode;
    if (z>=Depth()) return outOfBoundsNode;
    return nodes[x][z];
}

void Map::Randomize(float minHeight, float maxHeight) {
    
    for (int z =0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            Node& node = GetNode(x, z);
            node.height = minHeight + (maxHeight - minHeight) * MathHelper::Random();
            node.color = Colour::White();// Colour::HslToRgb(x*2+z*2, 1.0, 1.0, 1.0);
        }
    }
}

void Map::SetMaxHeight(float height) {
 for (int z =0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            Node& node = GetNode(x, z);
            if (node.height>height) node.height = height;
        }
    }
    CalcNormals();
}

void Map::SetHeight(float height) {
 for (int z =0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            Node& node = GetNode(x, z);
            node.height = height;
        }
    }
    CalcNormals();
}

void Map::SetEdges(float height) {
 for (int z = 0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            if (x>0 && z>0 && x<Width() -1 && z<Depth()-1) continue;
            Node& node = GetNode(x, z);
            node.height = height;
        }
    }
    CalcNormals();
}

void Map::Smooth(int iterations) {
    
    for (int i=0; i<iterations; i++) {
    
        for (int z =0; z<Depth(); z++) {
            for (int x=0; x<Width(); x++) {
            
                float totalHeight = 0;
                for (int zz=-1; zz<=1; zz++) {
                for (int xx=-1; xx<=1; xx++) {
                    Node& node = GetNode(x+xx, z+zz);
                    totalHeight+=node.height;
                }
                }
                Node& node = GetNode(x, z);
                node.height = totalHeight * 0.1111111111f;
            }
        }
    }
    CalcNormals();
}

void Map::CalcNormals() {
    CalcNormals({0,0,Width(), Depth()});
}

void Map::CalcNormals(const MapRect &rect) {
    
    for (int z = rect.z; z<rect.z + rect.depth; z++) {
        for (int x = rect.x; x<rect.x + rect.width; x++) {
            Node& node = GetNode(x, z);
            Node& right = GetNode(x+1, z);
            Node& down = GetNode(x, z+1);
            Node& left = GetNode(x-1, z);
            Node& up = GetNode(x, z-1);
            
            Vector3 position(x,node.height,z);
            Vector3 rightPosition(x+1,right.height, z);
            Vector3 downPosition(x,down.height, z+1);
            Vector3 leftPosition(x-1,left.height, z);
            Vector3 upPosition(x,up.height, z-1);
            
            Vector3 rightDir = (rightPosition - position);
            Vector3 downDir = (downPosition - position);
            
            Vector3 leftDir = leftPosition - position;
            Vector3 upDir = upPosition - position;
            
            upDir.Cross(leftDir, node.normal);
            node.normal += rightDir.Cross(upDir);
            node.normal += downDir.Cross(rightDir);
            node.normal += leftDir.Cross(downDir);
            node.normal = -node.normal.Normalized();
            
            //node.normal = rightDir.Cross(downDir).Normalized();
            //upDir.Cross(leftDir, node.normal);
            //node.normal += rightDir.Cross(upDir);
            //node.normal += downDir.Cross(rightDir);
            //node.normal += leftDir.Cross(downDir);
            
        }
    }
    
    for (int i=0; i<0; i++) {
    
    for (int z = rect.z; z<rect.depth; z++) {
        for (int x = rect.x; x<rect.width; x++) {
    
            Vector3 normal = 0;
            for(int xx=-1; xx<=1; xx++) {
                for(int zz=-1; zz<=1; zz++) {
                    normal += GetNode(x+xx, z+zz).normal;
                }
            }
            GetNode(x, z).normal = normal.Normalized();
        }
    }
    }
}

void Map::AddHill(int xPos, int zPos, int radius, float height)
{
    float radiusFloat = (float)radius;
    for (int z = -radius; z <= radius; z++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            int xx = xPos + x;
            int zz = zPos + z;
            
            Node& node = GetNode(xx, zz);
            
            if (&node!=&outOfBoundsNode)
            {
                float rad = sqrtf(x * x + z * z);
                
                if (rad <= radiusFloat)
                {
                    float radPercent = rad / radiusFloat;
                    float sinHeight = cosf(radPercent * M_PI_2);
                    node.height += sinHeight*height;
                }
            }
            
        }
    }
    CalcNormals({xPos - radius, zPos - radius, radius + radius, radius + radius});
}

void Map::CalculatePath(Point start, Point end, std::vector<Vector3> &path) {

    static const Point directions[] = {
        {1,0}, {1,1}, {0,1}, {-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}
    };
    static const int directionCosts[] = {
        10,14,10,14,10,14,10,14
    };

    static int pathId = 0;
    std::vector<Node*> openList;
    AddToOpenList(start.x, start.y, openList, pathId);
    
    Node* targetNode = 0;
    
    int inClosedListID = pathId + 1;
    
    while (true) {
        if (openList.empty()) break;
        
        std::sort(openList.begin(), openList.end(), SortNodes);
        
        Node* currentNode = openList.back();
        currentNode->pathId = inClosedListID;
        openList.pop_back();
        if (currentNode->x == end.x && currentNode->z == end.y) {
            targetNode = currentNode;
            break;
        }
        
        for (int i=0; i<8; i++) {
            int x = currentNode->x + directions[i].x;
            int z = currentNode->z + directions[i].y;
            
            Node* neighbor = &GetNode(x, z);
            if (!IsNodeWalkable(neighbor)) continue;
            if (neighbor->pathId == inClosedListID) continue;
            
            
            if (neighbor->pathId < pathId) {//not in open list
                AddToOpenList(x,z, openList, pathId);
                neighbor->parent = currentNode;
                neighbor->h = abs(end.x - x) * 10 + abs(end.y - z) * 10;
                neighbor->g = currentNode->g + directionCosts[i];
            } else {// in the open list
            
                int tempCost = (abs(x-currentNode->x) == 1 && abs(z - currentNode->z) == 1) ? 14 : 10;
                tempCost += currentNode->g;
            
                if (tempCost<neighbor->g) {
                    neighbor->parent = currentNode;
                    neighbor->g = tempCost;
                }
            }
        }
    }
    
    while (targetNode) {
        path.push_back({ (float)targetNode->x, targetNode->height, (float)targetNode->z });
        targetNode = targetNode->parent;
    }
    pathId+=2;
}

void Map::AddToOpenList(int x, int z, std::vector<Node*>& openList, int pathID) {
    Node& node = GetNode(x, z);
    node.pathId = pathID;
    node.x = x;
    node.z = z;
    node.g = 0;
    node.h = 0;
    node.parent = 0;
    openList.push_back(&node);
}

bool Map::IsNodeWalkable(Map::Node *node) {
    return node->height>0.48f;// && node->height<1.0f;
}

bool Map::IsNodeWalkable(const Map::Node& node) {
    return node.height>0.48f && node.height<1.3f;
}

bool Map::SortNodes(const Map::Node* a, const Map::Node* b) {
    return (a->g + a->h)>(b->g+b->h);
}

std::vector<Vector2> Map::CreateNavigationMesh() {
    
    //auto islands = FindIslands();
    
    /*
    vector<vector<c2t::Point>> inputPolygons(islands.size());
    
    for (int i=0; i<islands.size(); i++) {
        auto& hole = inputPolygons[i];
        hole.resize(islands[i].size());
        for (int j=0; j<hole.size(); j++) {
            hole[j].x = islands[i][j]->x;
            hole[j].y = islands[i][j]->z;
        }
    }
    */
    
    vector<vector<c2t::Point>> inputPolygons;
    
    
    for (int z=0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            const Node& node = GetNode(x, z);
            if (!IsNodeWalkable(node)) {
                inputPolygons.push_back({
                    { (float)x, (float)z },
                    { (float)x + 1.0f, (float)z },
                    { (float)x + 1.0f, (float)z + 1.0f },
                    { (float)x, (float)z + 1.0f },
                });
            }
        }
    }
    
    const float epsilon = 0.0f;
    vector<c2t::Point> outer { { epsilon,epsilon }, {epsilon,(float)Depth()-epsilon}, {(float)Width()-epsilon,(float)Depth()-epsilon}, {(float)Width()-epsilon,epsilon}};
    
    c2t::clip2tri clipper;
    
    vector<c2t::Point> navigationMesh;
    
    clipper.triangulate(inputPolygons, navigationMesh, outer);

    std::vector<Vector2> mesh(navigationMesh.size());
    for (int i=0; i<mesh.size(); i++) {
        mesh[i].x = navigationMesh[i].x;
        mesh[i].y = navigationMesh[i].y;
    }

    return mesh;
}

std::vector<std::vector<Map::Node*>> Map::FindIslands() {
    const Point dirs[] = {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};

    int islandIndex = 1;
    std::vector<std::vector<Node*>> islands;
    for (int z=0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            const Node& node = GetNode(x, z);
            if (node.islandID>0) continue;
            if (IsNodeWalkable(node)) continue; //walkable
            
            bool allBlocked = true;
            for (int i=0; i<8; i++) {
                const Point& p = dirs[i];
                const Node& neighbor = GetNode(x + p.x, z + p.y);
                if (!IsNodeWalkable(neighbor) || &neighbor == &outOfBoundsNode) {
                    allBlocked = false;
                    break;
                }
            }
            if (!allBlocked) {
                auto island = TraceIsland(x, z, islandIndex);
                islands.push_back(island);
                islandIndex++;
            }
        }
    }
    for (int z=0; z<Depth(); z++) {
        for (int x=0; x<Width(); x++) {
            GetNode(x, z).islandID = 0;
        }
    }
    
    return islands;
}

std::vector<Map::Node*> Map::TraceIsland(int x, int z, int& islandIndex) {
    std::vector<Node*> contour;
    
    const Point fourOffsets[] = {{-1,-1}, {0,-1},{-1,0},{0,0}};
    const Point contourDirs[] = {
        {0,0},//0
        {0,-1},//1
        {1,0},//2
        {1,0},//3
        {-1,0},//4
        {0,-1},//5
        {-1,0},//6
        {1,0},//7
        {0,1},//8
        {0,-1},//9
        {0,1},//10
        {0,1},//11
        {-1,0},//12
        {0,-1},//13
        {-1,0},//14
        {0,0},//15
    };
    
    int endX = x;
    int endZ = z;
    //std::cout <<" end pos : "<< endX << "," << endZ<< std::endl;
    int maxIter = 0;
    while (true) {
        int contourIndex = 0;
        for (int i=0; i<4; i++) {
            int xx = x + fourOffsets[i].x;
            int zz = z + fourOffsets[i].y;
            Node& node = GetNode(xx,zz);
            if (!IsNodeWalkable(node) || &node == &outOfBoundsNode) { // not walkable
                contourIndex |= 1<<i;
            } else {
                if (node.islandID==0) {
                    node.x = xx;
                    node.z = zz;
                    contour.push_back(&node);
                }
            }
            node.islandID = islandIndex;
        }
        
        const Point dir = contourDirs[contourIndex];
        x+=dir.x;
        z+=dir.y;
        
        //std::cout << x << "," << z<< std::endl;
        if (x == endX && z == endZ) {
            break;
        }
        maxIter++;
        if (maxIter>50) {
            break;
        }
    }
    for (Node* node : contour) {
        node->islandID = 0;
    }
    return contour;
}


