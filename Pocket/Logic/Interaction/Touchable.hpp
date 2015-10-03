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
        Pocket::GameObject* object;
        Pocket::Touchable* Touchable;
        Pocket::InputManager* Input;
        int Index;
        Pocket::Vector2 Position;
        Pocket::Vector3 WorldPosition;
        size_t TriangleIndex;
        Pocket::GameObject* CameraObject;
        Pocket::Camera* Camera;
        Pocket::Vector3 WorldNormal;
        Pocket::Ray Ray;
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
    
        SERIALIZE_FIELDS_BEGIN
        SERIALIZE_FIELD(ClickThrough);
        SERIALIZE_FIELDS_END
	};
}