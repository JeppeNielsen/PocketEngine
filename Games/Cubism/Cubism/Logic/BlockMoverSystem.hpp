//
//  BlockRotaterSystem.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Block.hpp"
#include "Touchable.hpp"
#include <map>

using namespace Pocket;

class BlockMoverSystem : public GameSystem {
    
public:
    
    GameObject* space;
    
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    
    void BlockDown(TouchData touch, GameObject* object);
    void BlockUp(TouchData touch, GameObject* object);
    
    
    
    struct MovingBlock {
        GameObject* go;
        Vector3 localPosition;
        TouchData touch;
        Vector2 touchOffset;
    };
    
    typedef std::map<GameObject*, MovingBlock> MovingBlocks;
    MovingBlocks movingBlocks;
    
};