//
//  MapMeshSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "MapMeshSystem.h"

void MapMeshSystem::ObjectAdded(Pocket::GameObject *object) {
	changedTransforms.Add(object->GetComponent<Transform>()->Position, object);
}

void MapMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
	changedTransforms.Remove(object->GetComponent<Transform>()->Position, object);
}

static int offsetX[4] = {0, 1, 1, 0};
static int offsetZ[4] = {0, 0, 1, 1};
static Vector3 lightDirection = Vector3(-1,-1,-1).Normalized();

void MapMeshSystem::Update(float dt) {
	if (changedTransforms.Objects().empty()) return;
    
    for	(auto* object : changedTransforms.Objects()) {
    	Map* map = object->GetComponent<Map>();
        MapRenderer* mapRenderer = object->GetComponent<MapRenderer>();
        Mesh* mesh = object->GetComponent<Mesh>();
        Transform* transform = object->GetComponent<Transform>();
        
        auto& verts = mesh->GetMesh<Vertex>().vertices;
        auto& triangles = mesh->GetMesh<Vertex>().triangles;
        
        if (verts.empty()) {
        	int numberOfVerts = (mapRenderer->width + 1) * (mapRenderer->depth+1);
        	verts.resize(numberOfVerts);
        	int numberOfTriangles = 6 * mapRenderer->width * mapRenderer->depth;
            triangles.resize(numberOfTriangles);
            
            int index = 0;
            for (int z = 0; z<mapRenderer->depth; z++) {
                for (int x = 0; x<mapRenderer->width; x++) {
                    short vert = x + z * (mapRenderer->width + 1);
                    short down = vert + mapRenderer->width + 1;
                    triangles[index++] = vert;
                    triangles[index++] = down;
                    triangles[index++] = vert+1;
                    
                    
                    triangles[index++] = vert+1;
                    triangles[index++] = down;
                    triangles[index++] = down+1;
                }
            }
        }
        
        Vector3 position = transform->Position;
        
        int posx = floorf(position.x);
        int posz = floorf(position.z);
        
        Vector3 offset(-(position.x-posx), 0, -(position.z-posz));
        
        for	(int z=0; z<=mapRenderer->depth; z++) {
        	for	(int x=0; x<=mapRenderer->width; x++) {
            
                int ox = x - mapRenderer->width / 2;
                int oz = z - mapRenderer->depth / 2;
            
                int xx = (int)floorf(position.x) + ox;
                int zz = (int)floorf(position.z) + oz;
                
                int index = x + z * (mapRenderer->width + 1);
            
                Map::Node& node = map->GetNode(xx, zz);
                Vertex& vertex = verts[index];
                vertex.Position = Vector3(ox, node.height, oz) + offset;
                vertex.Color = node.color;
                vertex.Normal = node.normal;
                vertex.TextureCoords =Vector2(xx*0.2f, zz*0.2f);
            }
        }
    }
}