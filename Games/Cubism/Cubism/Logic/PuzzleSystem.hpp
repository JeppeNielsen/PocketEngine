//
//  PuzzleSystem.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Puzzle.hpp"
#include "GameWorld.hpp"
#include "Patch.hpp"
#include "Block.hpp"
#include "Transform.hpp"

using namespace Pocket;

class PuzzleSystem : public GameSystem {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);

    void CreatePuzzle(const Puzzle& puzzle);
    
    void MoveBlocks(GameObject* object);
    
private:
    
    void CreatePatchFromPoint(const Puzzle& puzzle, const Point3& point, std::vector<GameObject*>& patches);
    
    bool GridIntersects(GameObject* patch, Block* block, const std::vector<GameObject*>& patches, int gridIndex);
    
    std::vector<GameObject*> patches;
    
};

