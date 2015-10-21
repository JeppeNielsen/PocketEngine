//
//  Map.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Map.h"
#include "MathHelper.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

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

void Map::CalculatePath(Vector3 start, Vector3 end, std::vector<Vector3> &path, GameWorld* world) {
    
    Vector2 startPosition = {start.x, start.z};
    Vector2 endPosition = {end.x, end.z};
    
    Vector2 nearestStartPosition;
    NavTriangle* startTriangle = navMesh.FindNearestTriangle(startPosition, nearestStartPosition);
    if (!startTriangle) return;
    
    Vector2 nearestEndPosition;
    NavTriangle* endTriangle = navMesh.FindNearestTriangle(endPosition, nearestEndPosition);
    if (!endTriangle) return;
    
    auto trianglePath = navMesh.FindPath(startTriangle, nearestStartPosition, endTriangle, nearestEndPosition);
    auto straightPath = navMesh.FindStraightPath(trianglePath);
    
    for(int i=((int)straightPath.size())-1; i>=0; --i) {
        path.push_back({ straightPath[i].x,0, straightPath[i].y});
    }
}

void Map::CreateTriangleObject(NavTriangle *tri, GameWorld* world) {
    GameObject* go = world->CreateObject();
    go->AddComponent<Transform>();
    auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
    for (int i=0; i<3; i++) {
        Vertex v;
        v.Position = {tri->corners[i].x, 2.0f, tri->corners[i].y};
        v.Color = Colour::Red();
        mesh.vertices.push_back(v);
    }
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(1);
    go->AddComponent<Material>()->Shader = &renderSystem->Shaders.Colored;
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

void Map::CreateNavigationMesh() {
    
    /*
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

    navMesh.Build(mesh);
    */
 
    /*
    std::vector<Vector2> mesh = navMesh.BuildPoints(Width(), Depth(), [this] (int x, int z) -> bool {
        return !IsNodeWalkable(GetNode(x, z));
    });
    */
    
    navMesh.BuildPointsTriangle(Width(), Depth(), [this] (int x, int z) -> bool {
        return !IsNodeWalkable(GetNode(x, z));
    });
}

const NavMesh& Map::NavMesh() { return navMesh; }

Vector3 Map::FindNearestValidPosition(const Pocket::Vector3 &position) {
    Vector2 nearest;
    navMesh.FindNearestTriangle({position.x, position.z}, nearest);
    return { nearest.x, 0, nearest.y };
}
