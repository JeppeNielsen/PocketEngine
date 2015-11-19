//
//  BlockPatchSnapSystem.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Block.hpp"
#include "Draggable.hpp"
#include "OctreeSystem.hpp"
#include "Patch.hpp"
#include "Camera.hpp"
#include "Touchable.hpp"
#include <map>

using namespace Pocket;

class BlockPatchSnapSystem : public GameSystem {
public:
    GameObject* cameraObject;
    Camera* camera;
    
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void Update(float dt);

    void AddedToWorld(GameWorld& world);
    
private:
    
    struct LinePick {
        GameObject* go;
        float distance;
        Vector2 screenTouchOffset;
    };
    
    static bool Sort(const LinePick& a, const LinePick& b);
    
    struct TouchingBlock {
        GameObject* go;
        TouchData touch;
        Vector2 screenSpaceCenterOffset;
    };
    
    void Down(TouchData d);
    void Up(TouchData d);
    
    void UpdateTouchingBlock(TouchingBlock& touchingBlock);
    
    typedef std::map<Touchable*, TouchingBlock> TouchingBlocks;
    TouchingBlocks touchingBlocks;
    
    OctreeSystem* octree;
};