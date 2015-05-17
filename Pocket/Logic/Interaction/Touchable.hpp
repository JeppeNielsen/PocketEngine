//
//  Touchable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/16/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"
#include "Material.hpp"
#include "Vector3.hpp"
#include "Mesh.hpp"

namespace Pocket {
    
    class Camera;
    class Touchable;
    class InputManager;
    struct TouchData {
        GameObject* object;
        Touchable* Touchable;
        InputManager* Input;
        int Index;
        Vector2 Position;
        Vector3 WorldPosition;
        size_t TriangleIndex;
        GameObject* CameraObject;
        Camera* Camera;
        Vector3 WorldNormal;
    };
    
	Component(Touchable)
	public:
		Touchable();
		~Touchable();
        
        void Reset();
        
        void Clone(const Touchable& source);
            
        Event<TouchData> Down;
        Event<TouchData> Up;
        Event<TouchData> Click;
    
        bool ClickThrough;
    
        Event<Touchable*> Cancelled;
    
        void Cancel();
    
	};
    
    
}