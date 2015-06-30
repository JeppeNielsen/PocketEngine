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
        
        Mesh::VerticesList& verts = mesh->Vertices();
        Mesh::TrianglesList& triangles = mesh->Triangles();
        
        if (verts.empty()) {
        	int numberOfVerts = 4 * mapRenderer->width * mapRenderer->depth;
        	verts.resize(numberOfVerts);
        	int numberOfTriangles = 6 * mapRenderer->width * mapRenderer->depth;
            triangles.resize(numberOfTriangles);
            
            int index = 0;
            for	(int i=0; i<numberOfVerts; i+=4) {
        		triangles[index++] = i;
                triangles[index++] = i+2;
                triangles[index++] = i+1;
                
                triangles[index++] = i;
                triangles[index++] = i+3;
                triangles[index++] = i+2;
            }
        }
        
        Vector3 position = transform->Position;
        
        int posx = floorf(position.x);
        int posz = floorf(position.z);
        
        Vector3 offset(-(position.x-posx), 0, -(position.z-posz));
        
        for	(int z=0; z<mapRenderer->depth; z++) {
        	for	(int x=0; x<mapRenderer->width; x++) {
            
                int ox = x - mapRenderer->width / 2;
                int oz = z - mapRenderer->depth / 2;
            
                int xx = (int)floorf(position.x) + ox;
                int zz = (int)floorf(position.z) + oz;
                
                int index = x * 4 + z * mapRenderer->width * 4;
                
                for (int i=0; i<4; i++) {
                    Map::Node& node = map->GetNode(xx + offsetX[i], zz + offsetZ[i]);
                    Vertex& vertex = verts[index + i];
                    vertex.Position = Vector3(ox + offsetX[i], node.height, oz + offsetZ[i]) + offset;
                    //float dot = lightDirection.Dot(node.normal);
                    //if (dot<0) dot = 0;
                    vertex.Color = node.color;
                    vertex.Normal = node.normal;
                }
                
                
            }
        }
        
        
    }
}