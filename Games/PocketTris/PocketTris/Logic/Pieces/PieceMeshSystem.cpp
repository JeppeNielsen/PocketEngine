//
//  PieceMeshSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceMeshSystem.hpp"

void PieceMeshSystem::Initialize() {
    AddComponent<Piece>();
    AddComponent<PieceStyle>();
    AddComponent<Mesh>();
}

void PieceMeshSystem::ObjectAdded(Nano::GameObject *object) {
    Piece* piece = object->GetComponent<Piece>();
    Mesh* mesh = object->GetComponent<Mesh>();
    PieceStyle* style = object->GetComponent<PieceStyle>();
    mesh->Clear();
    for (int x = 0; x<4; x++) {
        for (int y=0; y<4; y++) {
            if (piece->grid[x][y]) {
                mesh->AddCube(Vector3(x - piece->pivotX, y - piece->pivotY,0), 0.5f, style->color);
                //mesh->AddGeoSphere(Vector3(x - piece->pivotX, y - piece->pivotY,0), 0.5f, 4);
                mesh->SetColor(style->color);
            }
        }
    }
}

void PieceMeshSystem::ObjectRemoved(Nano::GameObject *object) {
    
}