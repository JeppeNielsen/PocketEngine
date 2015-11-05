//
//  HydralicMeshSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "HydralicMeshSystem.h"
#include "MathHelper.hpp"
#include "Atlas.hpp"
#include "Particle.h"

void HydralicMeshSystem::Initialize() {
    AddComponent<Spring>();
    AddComponent<Mesh>();
    AddComponent<Atlas>();
    AddComponent<Hydralic>();
}

void HydralicMeshSystem::ObjectAdded(GameObject* object) {
    
}

void HydralicMeshSystem::Update(float dt) {

    
    for (GameObject* object : Objects()) {
        const Atlas::Node& housing = object->GetComponent<Atlas>()->GetNode("HydralicHousing");
        const Atlas::Node& cylinder = object->GetComponent<Atlas>()->GetNode("HydralicMetal");
        
        auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
        Spring* spring = object->GetComponent<Spring>();
        
        float houseWidth = object->GetComponent<Hydralic>()->minLength;
        float houseEdgeWidth = 0.02f;
        float houseThickness = 1.2f;
        float houseTex = 0.05f;
        
        float cylinderEdgeWidth = 0.02f;
        float cylinderWidth = spring->length - houseWidth;
        float cylinderThickness = 0.80f;
        float cylinderTex = 0.05f;
        
        mesh.vertices.resize(6 + 8);
        
        mesh.vertices[0].Position = {cylinderThickness, houseWidth,0};
        mesh.vertices[0].TextureCoords = {cylinder.outer.left +cylinderTex,cylinder.outer.top};
        
        mesh.vertices[1].Position = {-cylinderThickness, houseWidth,0};
        mesh.vertices[1].TextureCoords = {cylinder.outer.left +cylinderTex,cylinder.outer.bottom};
        
        mesh.vertices[2].Position = {cylinderThickness, houseWidth+cylinderWidth-cylinderEdgeWidth,0};
        mesh.vertices[2].TextureCoords = {cylinder.outer.right - cylinderTex, cylinder.outer.top};
        
        mesh.vertices[3].Position = {-cylinderThickness,houseWidth+cylinderWidth-cylinderEdgeWidth,0};
        mesh.vertices[3].TextureCoords = {cylinder.outer.right - cylinderTex, cylinder.outer.bottom};
        
        mesh.vertices[4].Position = {cylinderThickness,houseWidth+cylinderWidth,0};
        mesh.vertices[4].TextureCoords = {cylinder.outer.right, cylinder.outer.top};
        
        mesh.vertices[5].Position = {-cylinderThickness,houseWidth+cylinderWidth,0};
        mesh.vertices[5].TextureCoords = {cylinder.outer.right, cylinder.outer.bottom};
       
        mesh.triangles.clear();
        
        for (int i=0; i<2; i++) {
            int index = i * 2;
            mesh.triangles.push_back(index + 0);
            mesh.triangles.push_back(index + 2);
            mesh.triangles.push_back(index + 3);
            
            mesh.triangles.push_back(index + 0);
            mesh.triangles.push_back(index + 3);
            mesh.triangles.push_back(index + 1);
        }
        
        mesh.vertices[6].Position = {houseThickness,0,0};
        mesh.vertices[6].TextureCoords = {housing.outer.left, housing.outer.top};
        
        mesh.vertices[7].Position = {-houseThickness,0,0};
        mesh.vertices[7].TextureCoords = {housing.outer.left, housing.outer.bottom};
        
        mesh.vertices[8].Position = {houseThickness,houseEdgeWidth,0};
        mesh.vertices[8].TextureCoords = {housing.outer.left+houseTex, housing.outer.top};
        
        mesh.vertices[9].Position = {-houseThickness, houseEdgeWidth,0};
        mesh.vertices[9].TextureCoords = {housing.outer.left+houseTex, housing.outer.bottom};
        
        mesh.vertices[10].Position = {houseThickness, houseWidth - houseEdgeWidth,0};
        mesh.vertices[10].TextureCoords = {housing.outer.right-houseTex, housing.outer.top};
        
        mesh.vertices[11].Position = {-houseThickness,houseWidth - houseEdgeWidth,0};
        mesh.vertices[11].TextureCoords = {housing.outer.right-houseTex, housing.outer.bottom};
        
        mesh.vertices[12].Position = {houseThickness, houseWidth,0};
        mesh.vertices[12].TextureCoords = {housing.outer.right, housing.outer.top};
        
        mesh.vertices[13].Position = {-houseThickness, houseWidth,0};
        mesh.vertices[13].TextureCoords = {housing.outer.right, housing.outer.bottom};
        
        for (int i=0; i<3; i++) {
            int index = 6 + i * 2;
            mesh.triangles.push_back(index + 0);
            mesh.triangles.push_back(index + 2);
            mesh.triangles.push_back(index + 3);
            
            mesh.triangles.push_back(index + 0);
            mesh.triangles.push_back(index + 3);
            mesh.triangles.push_back(index + 1);
        }
        
        
    }
}
