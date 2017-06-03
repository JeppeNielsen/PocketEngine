//
//  EditorSizeableSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "EditorSelectionSystem.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include "LineRenderer.hpp"
#include <set>

class EditorSizeableSelection : public EditorSelectionSystem<Transform, Sizeable> {
private:
    class SizeableLineRenderer : public GameSystem<LineRenderer, Sizeable> {
    protected:
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void SizeChanged(GameObject* object);
        void Update(float dt) override;
    private:
        std::set<GameObject*> dirtyObjects;
    };

public:
    void Initialize() override;
    void CreateObject(GameObject* object, GameObject* selectionObject) override;
};
