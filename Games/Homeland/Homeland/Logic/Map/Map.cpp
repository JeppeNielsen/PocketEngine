//
//  Map.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Map.h"
#include "MathHelper.hpp"
#include "Timer.hpp"

void Map::Reset() {
    navMesh = 0;
}

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

NavTriangle* Map::CalculatePath(Vector2 start, Vector2 end, std::vector<Vector2> &path, int& navMeshVersion, NavTriangle* hintStartTriangle) {
    if (!navMesh) return 0;
    
    Vector2 nearestStartPosition;
    NavTriangle* startTriangle = navMesh->FindNearestTriangle(navMesh->navigation, start, nearestStartPosition, navMeshVersion, hintStartTriangle);
    if (!startTriangle) return 0;
    
    Vector2 nearestEndPosition;
    NavTriangle* endTriangle = navMesh->FindNearestTriangle(navMesh->navigation, end, nearestEndPosition, navMeshVersion);
    if (!endTriangle) return 0;
    
    auto trianglePath = navMesh->FindPath(navMesh->navigation, startTriangle, nearestStartPosition, endTriangle, nearestEndPosition);
    auto straightPath = navMesh->FindStraightPath(navMesh->navigation, trianglePath);
    
    for(int i=((int)straightPath.size())-1; i>=0; --i) {
        path.push_back(straightPath[i]);
    }
    
    return startTriangle;
}

bool Map::IsNodeWalkable(const Map::Node& node) {
    if (node.obstacles>0) {
        return false;
    }
    float angle = MathHelper::RadToDeg * acosf(node.normal.Dot({0,1,0}));
    return angle>160.0f && node.height>0.02f;
}

void Map::CreateNavigationMesh(class NavMesh& mesh) {
    Timer timer;
    timer.Begin();
    mesh.BuildPointsTriangle(Width(), Depth(), [this] (int x, int z) -> bool {
        return !IsNodeWalkable(GetNode(x, z));
    });
    double time = timer.End();
    std::cout << "Nav mesh generation time = " << time << std::endl;
}

NavMesh* Map::NavMesh() { return navMesh; }

void Map::SetNavMesh(class NavMesh* navMesh) {
    this->navMesh = navMesh;
    NavigationUpdated(this);
}
