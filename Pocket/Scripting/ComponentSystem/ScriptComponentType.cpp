//
//  ScriptComponentType.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ScriptComponentType.hpp"

using namespace Nano;
using namespace AngelScript;

void* ScriptComponentType::CloneComponent(void *source) {
    ScriptComponent* sourceComponent = (ScriptComponent*)source;
    ScriptComponent* component = (ScriptComponent*)AddComponent();
    
    component->object = (asIScriptObject*)scriptType->GetEngine()->CreateScriptObjectCopy(sourceComponent->object, scriptType);
    component->object->AddRef();
    return component;
}