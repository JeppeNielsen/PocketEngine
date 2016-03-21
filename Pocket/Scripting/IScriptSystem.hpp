//
//  IScriptSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>

namespace Pocket {
  
struct IGameObject;

struct IScriptSystem {
    virtual ~IScriptSystem() {}
    virtual void ObjectAdded(IGameObject* object) = 0;
    virtual void ObjectRemoved(IGameObject* object) = 0;
    virtual void Update(float dt) = 0;
    virtual int AddObject(IGameObject* object) = 0;
    virtual IGameObject* RemoveObject(int indexToRemove) = 0;
};

}
