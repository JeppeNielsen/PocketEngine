//
//  PuzzleSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PuzzleSystem.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Touchable.hpp"
#include "MathHelper.hpp"
#include "Draggable.hpp"
#include <sstream>

using namespace Pocket;

void PuzzleSystem::Initialize() {
    AddComponent<Patch>();
    AddComponent<Transform>();
}

void PuzzleSystem::ObjectAdded(Pocket::GameObject *object) {
    
}

void PuzzleSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}

void PuzzleSystem::CreatePuzzle(const Puzzle& puzzle) {
    
    MathHelper::SeedRandom(puzzle.seed);
    
    patches.clear();
    
    for (size_t i = 0; i<puzzle.points.size(); i++) {
        CreatePatchFromPoint(puzzle, puzzle.points[i], patches);
    }
    
    for (size_t i=0; i<patches.size(); i++) {
        GameObject* patch = patches[i];
        GameObject* go = World()->CreateObject();
        Block* block = go->AddComponent<Block>();
        go->AddComponent<Transform>();//->Position = {i * 5,0,5};
        go->AddComponent<Mesh>();
        //go->AddComponent<Material>();
        go->Parent = patch;
        //go->AddComponent<Touchable>();
        
        patch->GetComponent<Patch>()->block = go;
        
        GameObject* visual = World()->CreateObject();
        visual->AddComponent<Transform>();
        visual->AddComponent<Mesh>(go);
        visual->AddComponent<Material>();
        visual->Parent = go;
        visual->AddComponent<Touchable>();
    }
    
//##### EF012
//##### D   3
//##### C   4
//##### B   5
//##### A9876
    
    
    const float probabilities[] {
        1.0f, 0.0f,0.9f,0.0f,
        1.0f, 0.14f,0.9f,0.14f,
        1.0f, 0.14f,0.9f,0.14f,
        1.0f, 0.14f,0.9f,0.14f
    };
    
    //#####

    for (int j=0; j<16; j++) {
    
        for (int i=0; i<patches.size(); i++) {
            GameObject* patch = patches[i];
            Block* block = patch->GetComponent<Patch>()->block->GetComponent<Block>();
        
            if (!GridIntersects(patch, block, patches, j)) {
                block->SetGrid(j, MathHelper::Random()<0.5f);//probabilities[j]);
            }
            block->EnsureCornersAreValid();
        
        }
        /*
        for (int i=0; i<16; i++) {
            if (block->GetGrid(i)) {
                Vector3 pos = block->GetGridWorldPosition(i, *patch->GetComponent<Transform>()->World.GetValue());
                GameObject* go = World()->CreateObject();
                go->AddComponent<Transform>()->Position = pos;//Vector3(pos.x, pos.y, pos.z);
                go->AddComponent<Mesh>()->AddCube(0, {5.5f,0.5f,0.5f}, Colour::Yellow());
                go->AddComponent<Material>();
            }
        }
        */
    }
    
    for (int i=0; i<patches.size(); i++) {
        GameObject* patch = patches[i];
        Block* block = patch->GetComponent<Patch>()->block->GetComponent<Block>();
        
        for (int j=0; j<16; j++) {
            if (block->GetGrid(j)) continue;
            
            bool isCorner = j==2 || j==6 || j==10 || j==14;

            if (isCorner && (!block->GetGrid(j-1) && !block->GetGrid(j+1))) {
                continue;
            }

            if (!GridIntersects(patch, block, patches, j)) {
                block->SetGrid(j, true);
            }
        }
    }
    
}

void PuzzleSystem::MoveBlocks(GameObject* object) {
    
    for (size_t i = 0; i<patches.size(); i++) {
        GameObject* blockObject = patches[i]->GetComponent<Patch>()->block;
        
        //patches[i]->GetComponent<Patch>()->block = blockObject->GetComponent<Block>();
        //blockObject->GetComponent<Block>()->Patch = patches[i]->GetComponent<Patch>();
        
        blockObject->Parent = object;
        blockObject->GetComponent<Transform>()->Position = {(float)(i % 3) * 8 - 8, (float)((int)((float)i/3.0f)) * 8 - 4 + (i==1 || i==4 ? (i==1 ? -2 : 2) : 0),-13.0f};
        
    }
    
}


void PuzzleSystem::CreatePatchFromPoint(const Puzzle &puzzle, const Pocket::Point3 &point, std::vector<GameObject*>& patches) {
    
    const Point3 directions[] {
        Point3(1, 0, 0),
        Point3(0, 1, 0),
        Point3(0, 0, 1),
        Point3(-1, 0, 0),
        Point3(0, -1, 0),
        Point3(0, 0, -1),
    };
    
    const Quaternion rotations[] {
        Quaternion::LookAt(0, {-1,0,0}, {0,1,0}),
        Quaternion::LookAt(0, {0,-1,0}, {0,0,1}),
        Quaternion::LookAt(0, {0,0,-1}, {0,1,0}),
        Quaternion::LookAt(0, {1,0,0}, {0,1,0}),
        Quaternion::LookAt(0, {0,1,0}, {0,0,1}),
        Quaternion::LookAt(0, {0,0,1}, {0,1,0}),
    };
    
    //#########
    //#OOO#OOO#
    //#OOO#OOO#
    //#OOO#OOO#
    //#########
    
    for (int i=0; i<6; i++) {
        Point3 pointToTest = point + directions[i];
        if (!puzzle.Intersects(pointToTest)) {
            GameObject* go = World()->CreateObject();
            Transform* transform = go->AddComponent<Transform>();
            Point3 position = point * 4 + directions[i] * 2;
            transform->Position = Vector3(position.x, position.y, position.z);
            transform->Rotation = rotations[i];
            go->AddComponent<Mesh>();
            go->AddComponent<Material>();
            
            std::stringstream s;
            s<<"Patch #"<<patches.size();
    
            go->AddComponent<Patch>()->name = s.str();
            patches.push_back(go);
            
            /*
            GameObject* blockGO = World()->CreateObject();
            
            blockGO->AddComponent<Transform>();//->Position = transform->Position + Vector3(directions[i].x, directions[i].y, directions[i].z) * 2;
            //blockGO->GetComponent<Transform>()->Rotation = transform->Rotation;
            
            
            Block* block = blockGO->AddComponent<Block>();
            block->CreateRandom();
            
            blockGO->AddComponent<Mesh>();
            blockGO->AddComponent<Material>();
            blockGO->AddComponent<Touchable>();
            blockGO->AddComponent<Hierarchy>()->Parent = hierarchy;
            */
            
        }
    }
}

bool PuzzleSystem::GridIntersects(Pocket::GameObject *patch, Block* block, const std::vector<GameObject*>& patches, int gridIndex) {
    
    Transform* transform = patch->GetComponent<Transform>();
    Vector3 pointToTest = block->GetGridWorldPosition(gridIndex, *transform->World.GetValue());
    
    for (size_t i = 0; i<patches.size(); i++) {
        if (patches[i] == patch) continue;
        const Matrix4x4& worldInverse = *patches[i]->GetComponent<Transform>()->WorldInverse.GetValue();
        if (patches[i]->GetComponent<Patch>()->block->GetComponent<Block>()->PointIntersects(pointToTest, worldInverse)) {
            return true;
        }
    }
    return false;
}

