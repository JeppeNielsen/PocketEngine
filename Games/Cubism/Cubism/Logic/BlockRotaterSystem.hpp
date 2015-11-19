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
#include "Patch.hpp"
#include <map>

using namespace Pocket;

class BlockRotatorSystem : public GameSystem {
    
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    
    void BlockClicked(TouchData touch, GameObject* object);
    
    void BlockDown(TouchData touch, GameObject* object);
    void BlockUp(TouchData touch, GameObject* object);
    
    struct DragData {
        float distance;
        Vector3 lastPosition;
        Patch* lastPatch;
        float timer;
    };
    
    typedef std::map<GameObject*, DragData> DragDistances;
    DragDistances dragDistances;
    
};