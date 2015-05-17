//
//  DragSelector.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Selectable.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "RenderSystem.hpp"
#include "InputManager.hpp"
#include "DraggableSystem.hpp"
#include "SelectableCollection.hpp"

namespace Pocket {
    class DragSelector : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
        void Setup(const Box& viewport, InputManager& input);
    
        void Update(float dt);
        void Render();
        
        bool IsDragging();
        
        void AddedToWorld(GameWorld& world);
            
    private:
        GameWorld renderWorld;
        RenderSystem* renderer;
        
        GameObject* dragRectangle;
        
        int draggingIndex;
        Vector2 dragStart;
        
        void Down(TouchEvent e);
        void Up(TouchEvent e);
        
        InputManager* input;
        
        class CameraSystem : public GameSystem {
        public:
            void Initialize();
        };
        
        CameraSystem* cameraSystem;
        
        void SelectObjects(Vector2 start, Vector2 end);
        void SelectObjectsFromCamera(GameObject* cameraObject, Vector2 start, Vector2 end);
        
        SelectableCollection* selectables;
        
        DraggableSystem* draggableSystem;
        bool isDraggableSystemChecked;
    };
}