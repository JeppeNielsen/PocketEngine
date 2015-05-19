//
//  PieceSpawnerSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceSpawnerSystem.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "PointTransform.hpp"
#include "PointGravity.hpp"
#include "PieceCollider.hpp"
#include "PieceController.hpp"
#include "MathHelper.hpp"

void PieceSpawnerSystem::Initialize() {
    AddComponent<Piece>();
    AddComponent<PieceController>();
    MathHelper::SeedRandom();
}

void PieceSpawnerSystem::Update(float dt) {
    if (Objects().empty()) {
        
        GameObject* pieceType = pieces[(size_t)MathHelper::Random((int)pieces.size())];
        
        GameObject* currentPiece = World()->CreateObject();
        currentPiece->AddComponent<Transform>();
        currentPiece->AddComponent<Material>();
        currentPiece->AddComponent<Mesh>();
        currentPiece->AddComponent<PointTransform>()->position = Point(0,20);
        currentPiece->AddComponent<Piece>(pieceType);
        currentPiece->AddComponent<PointGravity>()->Gravity = 3.0f;
        currentPiece->AddComponent<PieceCollider>();
        currentPiece->AddComponent<PieceController>();
        currentPiece->AddComponent<PieceStyle>(pieceType);//->color = Colour::HslToRgb(MathHelper::Random()*360, 1, 1, 1);
        
    }
}

void PieceSpawnerSystem::CreatePieces() {

    CreatePiece("#O##"
                ,Colour::Red()
                );
    
    CreatePiece(" #  "
                "#O# "
                ,Colour::Blue()
                );
    
    CreatePiece(" ## "
                "#O  "
                ,Colour::Yellow()
                );
    
    CreatePiece("##  "
                " O# "
                ,Colour::Green()
                );
    
    CreatePiece("##  "
                "O#  "
                ,Colour::White()
                );
    
    CreatePiece("#   "
                "O   "
                "##  "
                ,Colour::White()
                );
    CreatePiece(" #  "
                " O  "
                "##  "
                ,Colour::White()
                );
    /*
    CreatePiece(" #  "
                " O  "
                "### "
                ,Colour::White()
                );
    CreatePiece(" ###"
                " #O#"
                "####"
                ,Colour::White()
                );
*/
}

void PieceSpawnerSystem::CreatePiece(std::string s, Colour color) {
    GameObject* go = World()->CreateObject();
    go->AddComponent<Piece>()->SetFromString(s);
    PieceStyle* style = go->AddComponent<PieceStyle>();
    style->color = Colour::HslToRgb(pieces.size() * 360.0 / 7.0, 1, 1, 1);
    pieces.push_back(go);
}