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
    
    Colour color = Colour::HslToRgb(hue, MathHelper::Random(0.5f, 1.0f), 1.0, 1.0);
    
    for (int i=0; i<contour.size(); i++) {
        Vertex v;
        v.Position = { contour[i].x, contour[i].y, 0.5f };
        v.TextureCoords = { contour[i].x, contour[i].y };
        v.Normal = {0,0,1};
        v.Color = color;
        vertices.push_back(v);
     }
    
    for (int i=0; i<contour.size(); i++) {
        Vertex v;
        v.Position = { contour[i].x, contour[i].y, -0.5f };
        v.TextureCoords = { contour[i].x, contour[i].y };
        v.Normal = {0,0,-1};
        v.Color = color;
        vertices.push_back(v);
    }
    
    for (int i=0; i<indicies.size(); i++) {
        triangles.push_back((short)indicies[i]);
    }
    
    for (int i=indicies.size()-1; i>=0; i--) {
        triangles.push_back((short)contour.size() + indicies[i]);
    }

    
    for (int i=0; i<contour.size(); i++) {
        Vector2 next = i<contour.size()-1 ? contour[i + 1] : contour[0];
        Vector2 dir = next - contour[i];
        Vector2 normal = { -dir.y, dir.x};
        normal.Normalize();
        
        size_t offset = vertices.size();
    
        Vertex v1;
        v1.Position = { contour[i].x, contour[i].y, 0.5f };
        v1.TextureCoords = { contour[i].x, contour[i].y };
        v1.Normal = {normal.x,normal.y,0};
        v1.Color = color;
        vertices.push_back(v1);
        
        Vertex v2;
        v2.Position = { contour[i].x, contour[i].y, -0.5f };
        v2.TextureCoords = { contour[i].x, contour[i].y };
        v2.Normal = {normal.x,normal.y,0};
        v2.Color = color;
        vertices.push_back(v2);
        
        Vertex v3;
        v3.Position = { next.x, next.y, 0.5f };
        v3.TextureCoords = { next.x, next.y };
        v3.Normal = {normal.x,normal.y,0};
        v3.Color = color;
        vertices.push_back(v3);
        
        Vertex v4;
        v4.Position = { next.x, next.y, -0.5f };
        v4.TextureCoords = { next.x, next.y };
        v4.Normal = {normal.x,normal.y,0};
        v4.Color = color;
        vertices.push_back(v4);
        
        triangles.push_back((short)offset + 0);
        triangles.push_back((short)offset + 2);
        triangles.push_back((short)offset + 1);
        
        triangles.push_back((short)offset + 1);
        triangles.push_back((short)offset + 2);
        triangles.push_back((short)offset + 3);
    }
}