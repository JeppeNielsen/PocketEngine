//
//  EditorCameraSelection.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "EditorSelectionSystem.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "EditorProxyComponent.hpp"
#include "LineRenderer.hpp"
#include <set>

class EditorCameraSelection : public EditorSelectionSystem<Transform, EditorProxyComponent<Camera>> {
private:
    class CameraLineRenderer : public GameSystem<LineRenderer, EditorProxyComponent<Camera>> {
    protected:
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void CameraPropertyChanged(GameObject* object);
        void Update(float dt) override;
    private:
        std::set<GameObject*> dirtyObjects;
    };

public:
    void Initialize() override;
    void CreateObject(GameObject* object, GameObject* selectionObject) override;
    static void CreateSubSystems(GameStorage& storage);
};
