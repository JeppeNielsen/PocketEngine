//
//  BeamMeshSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BeamMeshSystem.h"
#include "MathHelper.hpp"
#include "Particle.h"

void BeamMeshSystem::ObjectAdded(GameObject* object) {
    
}

void BeamMeshSystem::Update(float dt) {

    
    for (GameObject* object : Objects()) {
        const Atlas::Node& node = object->GetComponent<Atlas>()->GetNode("Beam");
        auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
        Spring* spring = object->GetComponent<Spring>();
        
        const float segmentWidth = 2.2f;
        
        int numSegments = roundf(spring->length / segmentWidth);
        
        mesh.triangles.clear();
        
        for (int i=0; i<numSegments; i++) {
            int index = i * 4;
            mesh.triangles.push_back(index + 0);
            mesh.triangles.push_back(index + 2);
            mesh.triangles.push_back(index + 1);
            
            mesh.triangles.push_back(index + 0);
            mesh.triangles.push_back(index + 3);
            mesh.triangles.push_back(index + 2);
        }
        
        mesh.vertices.resize(numSegments * 4);
        
        float delta = spring->currentLength / numSegments;
    
        float normalizedTension = MathHelper::Clamp(spring->tension*0.5f, -1.0f, 1.0f);
        Colour color;
        if (normalizedTension<0) {
            color = Colour::Lerp(Colour(1.0f,1.0f,1.0f,1.0f), Colour(0.0f,0.0f,1.0f,1.0f), -normalizedTension);
        } else {
            color = Colour::Lerp(Colour(1.0f,1.0f,1.0f,1.0f), Colour(1.0f,0.0f,0.0f,1.0f), normalizedTension);
        }
        
        color = Colour::White();
        
        for (int i=0; i<numSegments; i++) {
            float position = delta * i;
            int index = i * 4;
            float percent = i / (float)(numSegments);
            float percent2 = (i+1) / (float)(numSegments);
            
            float width = 1.0f-sinf(percent * MathHelper::Pi)*normalizedTension*0.7f;
            float width2 = 1.0f-sinf(percent2 * MathHelper::Pi)*normalizedTension*0.7f;
            
            mesh.vertices[index+0].Position = {-width, position,0};
            mesh.vertices[index+1].Position = {-width2, position+delta,0};
            mesh.vertices[index+2].Position = {width2, position+delta,0};
            mesh.vertices[index+3].Position = {width, position,0};
            
            mesh.vertices[index+0].TextureCoords = { node.outer.left+0.0655f, node.outer.top };
            mesh.vertices[index+1].TextureCoords = { node.outer.right-0.0655f, node.outer.top };
            mesh.vertices[index+2].TextureCoords = { node.outer.right-0.0655f, node.outer.bottom };
            mesh.vertices[index+3].TextureCoords = { node.outer.left+0.0655f, node.outer.bottom };
            
            mesh.vertices[index+0].Color = color;
            mesh.vertices[index+1].Color = color;
            mesh.vertices[index+2].Color = color;
            mesh.vertices[index+3].Color = color;
        }
        
        
    }
}
