
//
//  ClonerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ClonerSystem.hpp"
#include "GameWorld.hpp"
#include <fstream>

using namespace Pocket;

void ClonerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.SetRoot(root);
    object->GetComponent<Cloner>()->Source.Changed.Bind(this, &ClonerSystem::CloneSourceChanged, object);
    if (object->Children().empty()) {
        CloneSourceChanged(object);
    }
}

void ClonerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.Changed.Unbind(this, &ClonerSystem::CloneSourceChanged, object);
    for(auto child : object->Children()) {
        child->Remove();
    }
}

void ClonerSystem::CloneSourceChanged(Pocket::GameObject *object) {
    Cloner* cloner = object->GetComponent<Cloner>();
    for(auto child : object->Children()) {
        child->Remove();
    }
    GameObject* source = cloner->Source;
    if (!source) return;
    object->CreateChildClone(source);
}


