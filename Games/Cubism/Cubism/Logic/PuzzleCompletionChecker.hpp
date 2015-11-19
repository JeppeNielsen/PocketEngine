//
//  PuzzleCompletionChecker.h
//  Cubism
//
//  Created by Jeppe Nielsen on 9/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Event.hpp"
#include "Block.hpp"

using namespace Pocket;

class PuzzleCompletionChecker : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    
        Event<bool> PuzzleComplete;

    private:
        void BlockChanged(Block* block);
        void CheckForCompletion();
    
};