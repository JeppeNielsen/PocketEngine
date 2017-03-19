//
//  EditorSelectionSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Selectable.hpp"
#include <functional>
#include <map>

using namespace Pocket;

template<typename ...T>
class EditorSelectionSystem : public GameSystem<Selectable, T...> {
protected:
    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &EditorSelectionSystem::SelectionChanged, object);
    }
    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &EditorSelectionSystem::SelectionChanged, object);
        TryRemoveObject(object);
    }
    
    void SelectionChanged(Pocket::GameObject *object) {
        if (object->GetComponent<Selectable>()->Selected) {
            GameObject* selectionObject = this->root->CreateObject();
            CreateObject(object, selectionObject);
            selectionObjects[object] = selectionObject;
        } else {
            TryRemoveObject(object);
        }
    }
    
    void TryRemoveObject(GameObject* object) {
        auto it = selectionObjects.find(object);
        if (it!=selectionObjects.end()) {
            RemoveObject(object, it->second);
            it->second->Remove();
            selectionObjects.erase(it);
        }
    }
    
    virtual void CreateObject(GameObject* object, GameObject* selectionObject) {}
    virtual void RemoveObject(GameObject* object, GameObject* selectionObject) {}
    
private:
    std::map<GameObject*, GameObject*> selectionObjects;
};
