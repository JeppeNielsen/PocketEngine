//
//  TerrainVertexEditor.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Terrain.h"
#include "TerrainEditableVertices.h"
#include "InputManager.hpp"
#include "TouchSystem.hpp"
#include "Selectable.hpp"
#include "SelectableCollection.hpp"

using namespace Pocket;

struct TerrainVertexEditor : public GameSystem<Terrain, Transform, TerrainEditableVertices> {
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    void SetInput(InputManager* input);
private:
    TouchSystem* touchSystem;
    GameObject* CreateVertexObject(Vector2 position);

    struct AddVertexSystem: public GameSystem<Terrain, Touchable, Transform, TerrainEditableVertices> {
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        void Down(TouchData d, GameObject* object);
        int FindInsertPosition(Terrain* terrain, const Vector2& position);
        float SegmentDistance(const Vector2& v, const Vector2& w, const Vector2& p);
    };

    void ButtonDown(std::string button);

    SelectableCollection<>* selectedVertices;
};