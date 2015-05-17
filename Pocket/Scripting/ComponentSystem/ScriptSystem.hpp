//
//  ScriptSystem.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/12/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//


#pragma once

#include "GameSystem.hpp"
#include <angelscript.h>
#include <string>

namespace AngelScript {
    class CSerializer;
}

namespace Nano {
  
class ScriptWorld;

class ScriptSystem : public GameSystem {
    
public:

    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    
    void Destroy();
    
    ScriptWorld* scriptWorld;
    
    AngelScript::asIScriptContext* context;
    AngelScript::asIScriptObject* scriptObject;
    
    AngelScript::asIScriptFunction* objectAddedMethod;
    AngelScript::asIScriptFunction* objectRemovedMethod;
    AngelScript::asIScriptFunction* updateMethod;
    
       
    void Store(AngelScript::CSerializer* serializer);
    void Restore(AngelScript::CSerializer* serializer);
    
    std::string name;
    std::string nameSpace;
    
};

}
