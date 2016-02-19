//
//  Touchable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/16/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Material.hpp"
#include "Vector3.hpp"
#include "Mesh.hpp"

namespace Pocket {
    
    class Camera;
    class Touchable;
    class InputManager;
    struct TouchData {
        //Pocket::GameObject* object;
        Pocket::Touchable* Touchable;
        Pocket::InputManager* Input;
        int Index;
        Pocket::Vector2 Position;
        Pocket::Vector3 WorldPosition;
        size_t TriangleIndex;
        Pocket::Transform* CameraTransform;
        Pocket::Camera* Camera;
        Pocket::Vector3 WorldNormal;
        Pocket::Ray Ray;
    };
    
	class Touchable {
	public:
        
        Touchable() : ClickThrough(false) { }

        void Cancel() {
            Cancelled(this);
        }
		
        Event<TouchData> Down;
        Event<TouchData> Up;
        Event<TouchData> Click;
    
        bool ClickThrough;
    
        Event<Touchable*> Cancelled;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(ClickThrough);
        TYPE_FIELDS_END
	};
}