//
//  BlockMeshSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/17/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockMeshSystem.hpp"
#include "Triangulator.hpp"
#include "MathHelper.hpp"

void BlockMeshSystem::Initialize() {
    AddComponent<Block>();
    AddComponent<Mesh>();
}

void BlockMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    CreateMesh(object->GetComponent<Block>(), object->GetComponent<Mesh>());
}

void BlockMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}

void BlockMeshSystem::CreateMesh(Block *block, Pocket::Mesh *mesh) {
    
    auto& vertices = mesh->GetMesh<Vertex>().vertices;
    auto& triangles = mesh->GetMesh<Vertex>().triangles;
    
    vertices.clear();
    triangles.clear();
    
    /*
    mesh->AddQuad(0, {3,3}, Colour::White());
    
    for (int i=0; i<16; i++) {
        if (block->grid[i]) {
            Vector2 pos = block->GetGridPosition(i);
            mesh->AddQuad(pos, 1, Colour::HslToRgb(360.0 / 16 * i, 1, 1, 1));
        }
    }
    
     */
    
    Triangulator::Vertices contour;
    Vector2 firstPosition;
    for (int i=0; i<16; i++) {
        Vector2 c = block->GetGridPosition(i);
        Vector2 n = block->GetGridNormal(i);
        Vector2 t = { n.y, -n.x };
        bool set = block->GetGrid(i);
        bool prevChange = set!=block->GetGrid(i - 1);
        bool nextChange = set!=block->GetGrid(i + 1);
        bool isCorner = i==2 || i==6 || i==10 || i == 14;
        
        Vector2 p = c + (set ? n : -n) * 0.5f - t * 0.5f;
        
        if (i==0) {
            firstPosition = p;
        }
        
        if (i==0 || prevChange) {
            contour.push_back(p);
        }
        
        if (isCorner) {
            
            if (set) {
                contour.push_back(p+t);
                if (nextChange) {
                    contour.push_back(p+t-n);
                }
            } else {
                if (!prevChange && !nextChange) {
                    contour.push_back(p);
                }
            }
        } else {
            if (nextChange) {
                contour.push_back(p+t);
            }
        }
    }
    
    Triangulator::IndiciesVector indicies;
    Triangulator::Triangulate(contour, indicies);
    
    //mesh->AddQuad(contour[0], 0.1f, Colour::White());
   // mesh->AddQuad(contour[contour.size()-1], 0.1f, Colour::White());
    
    double hue = MathHelper::Random() * 360; // = 240
    
    Colour color = Colour::HslToRgb(hue, 0.3 + MathHelper::Random() * 0.7, 1.0, 1.0);
    
    for (int i=0; i<contour.size(); i++) {
        vertices.push_back(Vertex(Vector3(contour[i].x, contour[i].y, 0.5f), color));
    }
    
    for (int i=0; i<contour.size(); i++) {
        vertices.push_back(Vertex(Vector3(contour[i].x, contour[i].y, -0.5f), color));
    }
    
    for (int i=0; i<indicies.size(); i++) {
        triangles.push_back((short)indicies[i]);
    }
    
    for (int i=indicies.size()-1; i>=0; i--) {
        triangles.push_back((short)contour.size() + indicies[i]);
    }
    
   color = Colour::HslToRgb(hue, 1.0, 0.5, 1.0);
    
    
    int offset = vertices.size();
    
    for (int i=0; i<contour.size(); i++) {
        vertices.push_back(Vertex(Vector3(contour[i].x, contour[i].y, 0.5f), color));
    }

    int size = contour.size();
    
    for (int i=0; i<contour.size(); i++) {
        vertices.push_back(Vertex(Vector3(contour[i].x, contour[i].y, -0.5f), color));
    }
    
    for (int i=0; i<contour.size() - 1; i++) {
        triangles.push_back((short)offset + i);
        triangles.push_back((short)offset + i + 1);
        triangles.push_back((short)offset + i + 1 + size);
        
        triangles.push_back((short)offset + i);
        triangles.push_back((short)offset + i + 1 + size);
        triangles.push_back((short)offset + i + size);
    }
    
    triangles.push_back((short)offset + contour.size()-1);
    triangles.push_back((short)offset + 0);
    triangles.push_back((short)offset + size);
    
    triangles.push_back((short)offset + contour.size()-1);
    triangles.push_back((short)offset + size);
    triangles.push_back((short)offset + contour.size()-1 + size);
    
    
    
    //for (int i=0; i<contour.size(); i++) {
    //    mesh->AddQuad(contour[i], 0.1f, Colour::Blue());
    //}
    
    
}