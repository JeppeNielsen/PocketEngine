//
//  SelectionCollection.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/26/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Selectable.hpp"

namespace Pocket {
    template<typename ...T>
    class SelectableCollection : public GameSystem<Selectable, T...>  {
    public:
    void Initialize() {
        hasChanged = false;
    }

    void ClearSelection() {
        for (auto o : this->Objects()) {
            o->template GetComponent<Selectable>()->Selected = false;
        }
        hasChanged = true;
    }
    
    void SelectedChanged(GameObject* object) {
        Selectable* selectable = object->GetComponent<Selectable>();
        if (selectable->Selected) {
            selectedObjects.push_back(object);
            hasChanged = true;
        } else {
           TryRemoveObject(object);
        }
    }

    void ObjectAdded(Pocket::GameObject *object) {
        object->template GetComponent<Selectable>()->Selected.Changed.Bind(this, &SelectableCollection<T...>::SelectedChanged, object);
        SelectedChanged(object);
    }

    void ObjectRemoved(Pocket::GameObject *object) {
        object->template GetComponent<Selectable>()->Selected.Changed.Unbind(this, &SelectableCollection<T...>::SelectedChanged, object);
        TryRemoveObject(object);
    }

    void TryRemoveObject(Pocket::GameObject *object) {
        auto i = std::find(selectedObjects.begin(), selectedObjects.end(), object);
        if (i!=selectedObjects.end()) {
            selectedObjects.erase(i);
            hasChanged = true;
        }
    }

    const ObjectCollection& Selected() { return selectedObjects; }

    void Update(float dt) {
        if (hasChanged) {
            hasChanged = false;
            SelectionChanged(this);
        }
    }
    
    Event<SelectableCollection*> SelectionChanged;
    
    private:
        ObjectCollection selectedObjects;
        bool hasChanged;
    };
}