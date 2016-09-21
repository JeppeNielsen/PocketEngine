
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
    object->GetComponent<Cloner>()->ClonePath.Changed.Bind(this, &ClonerSystem::ClonePathChanged, object);
    if (object->Children().empty()) {
        ClonePathChanged(object);
    }
}

void ClonerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->ClonePath.Changed.Unbind(this, &ClonerSystem::ClonePathChanged, object);
}

void ClonerSystem::ClonePathChanged(Pocket::GameObject *object) {
    for(auto child : object->Children()) {
        child->Remove();
    }
    
    std::string path = object->GetComponent<Cloner>()->ClonePath;
    
    std::fstream file;
    file.open(path);
    world->CreateObject(file, object);
    file.close();
}