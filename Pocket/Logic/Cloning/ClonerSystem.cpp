
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
    Cloner* cloner = object->GetComponent<Cloner>();
    
    for(int i : cloner->storedComponents) {
        object->RemoveComponent(i);
    }
    
    for(auto child : object->Children()) {
        child->Remove();
    }
    
    cloner->storedComponents.clear();
    std::string path = object->GetComponent<Cloner>()->ClonePath;
    
    std::vector<int> previousComponents = object->GetComponentIndicies();
    
    std::fstream file;
    file.open(path);
    world->CreateObject(file, object, 0, object);
    file.close();
    
    std::vector<int> afterComponents = object->GetComponentIndicies();
    
    for (auto i : afterComponents) {
        auto it = std::find(previousComponents.begin(), previousComponents.end(), i);
        if (it == previousComponents.end()) {
            cloner->storedComponents.push_back(i);
        }
    }
}