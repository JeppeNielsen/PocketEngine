//
//  BlockTransformer.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Block.hpp"

using namespace Pocket;

class BlockTransformer : public GameSystem {
    
public:
    void Initialize();
    
    void Update(float dt);
    
    
};
