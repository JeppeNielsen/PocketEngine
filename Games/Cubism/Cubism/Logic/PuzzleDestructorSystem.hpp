//
//  PuzzleDestructorSystem.h
//  Cubism
//
//  Created by Jeppe Nielsen on 9/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Block.hpp"
#include "Patch.hpp"

using namespace Pocket;

class PuzzleDestructorSystem : public GameSystem {
public:
    void AddedToWorld(GameWorld& world);
    void Initialize();
    void DestroyPuzzle();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    
private:
    
    class PatchDestructor : public GameSystem {
    public:
        void Initialize();
        void DestroyPatches();
        
    };
    PatchDestructor* patchDestructor;
    
};
