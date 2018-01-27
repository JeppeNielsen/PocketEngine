//
//  Hierarchy.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 05/01/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "Hierarchy.hpp"
#include "GameObject.hpp"
#include "GameScene.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

Hierarchy::Hierarchy() {
    owner = nullptr;
    Enabled = true;
    worldEnabled = true;
    childIndex = 0;
    Parent = nullptr;
    Parent.Changed.Bind([this] {
        GameObject* prev = Parent.PreviousValue();
        GameObject* current = Parent;
        
        if (prev) {
            Hierarchy* prevHierarchy = prev->GetComponent<Hierarchy>();
            if (prevHierarchy) {
                GameObject* lastChild = prevHierarchy->children.back();
                prevHierarchy->children[childIndex] = lastChild;
                lastChild->GetComponent<Hierarchy>()->childIndex = childIndex;
                prevHierarchy->children.pop_back();
            }
        }
        
        if (current) {
            Hierarchy* currentHierarchy = current->GetComponent<Hierarchy>();
            childIndex = (int)currentHierarchy->children.size();
            currentHierarchy->children.push_back(owner);
            CalculateEnabled(Parent(), owner);
        }
    });
    Parent.Changed.MarkDefaults();
    children.clear();
    Enabled.Changed.Bind([this] {
        if (owner) {
            CalculateEnabled(Parent(), owner);
        }
    });
    Enabled.Changed.MarkDefaults();
}

void Hierarchy::CalculateEnabled(GameObject* parent, GameObject *object) {
    Hierarchy* hierarchy = object->GetComponent<Hierarchy>();
    bool prevWorldEnabled = hierarchy->worldEnabled;

    if (parent) {
        Hierarchy* parentHierarchy = parent->GetComponent<Hierarchy>();
        hierarchy->worldEnabled = parentHierarchy->worldEnabled && hierarchy->Enabled();
    } else {
        hierarchy->worldEnabled = hierarchy->Enabled();
    }
    if (prevWorldEnabled!=worldEnabled) {
    
        object->scene->world->delayedActions.emplace_back([hierarchy](){
            hierarchy->owner->SetEnabled(hierarchy->worldEnabled);
        });
    
        hierarchy->WorldEnabledChanged();
        for(auto child : hierarchy->children) {
            CalculateEnabled(object, child);
        }
    }
}

Hierarchy::Hierarchy(const Hierarchy& other) {
    owner = nullptr;
    Enabled = true;
    worldEnabled = true;
    childIndex = 0;
    Parent = nullptr;
    Parent.Changed.Bind([this] {
        GameObject* prev = Parent.PreviousValue();
        GameObject* current = Parent;
        
        if (prev) {
            Hierarchy* prevHierarchy = prev->GetComponent<Hierarchy>();
            GameObject* lastChild = prevHierarchy->children.back();
            prevHierarchy->children[childIndex] = lastChild;
            lastChild->GetComponent<Hierarchy>()->childIndex = childIndex;
            prevHierarchy->children.pop_back();
        }
        
        if (current) {
            Hierarchy* currentHierarchy = current->GetComponent<Hierarchy>();
            childIndex = (int)currentHierarchy->children.size();
            currentHierarchy->children.push_back(owner);
            CalculateEnabled(Parent(), owner);
        }
    });
    Parent.Changed.MarkDefaults();
    children.clear();
    Enabled.Changed.Bind([this] {
        CalculateEnabled(Parent(), owner);
    });
    Enabled.Changed.MarkDefaults();
}

const Hierarchy::ChildrenCollection& Hierarchy::Children() {
    return children;
}

bool Hierarchy::WorldEnabled() const {
    return worldEnabled;
}
