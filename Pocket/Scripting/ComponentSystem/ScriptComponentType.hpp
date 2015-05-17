//
//  ScriptComponentType.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "ScriptComponent.hpp"
#include "GameComponentType.hpp"

namespace AngelScript {
  class asIObjectType;
}

namespace Nano {
  
class ScriptComponentType : public GameComponentType<ScriptComponent> {
    public:
        AngelScript::asIObjectType* scriptType;
        int index;
        void* CloneComponent(void* source);
};

}