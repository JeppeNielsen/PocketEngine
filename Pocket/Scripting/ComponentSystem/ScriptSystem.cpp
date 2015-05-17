//
//  ScriptSystem.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/12/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ScriptSystem.hpp"
#include <assert.h>
#include "ScriptWorld.hpp"
#include "serializer.h"

using namespace Nano;

void ScriptSystem::Initialize() {
    objectAddedMethod = 0;
    objectRemovedMethod = 0;
    updateMethod = 0;
}

void ScriptSystem::ObjectAdded(GameObject *object) {
    if (!objectAddedMethod) return;
    context->Prepare(objectAddedMethod);
    context->SetObject(scriptObject);
    context->SetArgObject(0, object);
    int r = context->Execute(); assert(r>=0);
}

void ScriptSystem::ObjectRemoved(GameObject *object) {
    if (!objectRemovedMethod) return;
    context->Prepare(objectRemovedMethod);
    context->SetObject(scriptObject);
    context->SetArgObject(0, object);
    int r = context->Execute(); assert(r>=0);
}

void ScriptSystem::Destroy() {
    scriptObject->Release();
}

void ScriptSystem::Update(float dt) {
    if (!updateMethod) return;
    scriptWorld->SetActiveSystem(this);
    context->Prepare(updateMethod);
    context->SetObject(scriptObject);
    context->SetArgFloat(0, dt);
    int r = context->Execute(); assert(r>=0);
    scriptWorld->SetActiveSystem(0);
}

void ScriptSystem::Store(AngelScript::CSerializer *serializer) {
    serializer->AddExtraObjectToStore(scriptObject);
}

void ScriptSystem::Restore(AngelScript::CSerializer *serializer) {
    scriptObject = reinterpret_cast<AngelScript::asIScriptObject*>(serializer->GetPointerToRestoredObject(scriptObject));
}