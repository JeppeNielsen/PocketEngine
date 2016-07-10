//
//  EditorFactory.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "EditorSpawner.h"

class EditorFactory : public GameConcept {
public:
    void Initialize();
    void Setup();
    
    GameObject* CreateTerrain(Vector3 position);
    GameObject* CreateStaticParticle(Vector3 position);
private:
    GameObject* terrainTexture;
    GameObject* dirtTexture;
    GameObject* atlas;
};