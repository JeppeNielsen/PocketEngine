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
#include "BlockMoverSystem.hpp"

using namespace Pocket;

class BlockParentSystem : public GameSystem {
    
public:
    
    GameObject* parent;
    
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    
    BlockMoverSystem* moverSystem;
    
private:
    
    void PatchChanged(Property<Block*, GameObject*>::EventData data, GameObject* object);
    
};