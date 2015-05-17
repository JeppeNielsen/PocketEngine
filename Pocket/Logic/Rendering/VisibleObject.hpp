//
//  VisibleObject.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "TextureComponent.hpp"
#include "Colorable.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Orderable.hpp"

namespace Pocket {
    class Shader;
    class GameObject;
    class VisibleObject {
    public:
    
        enum Type {
            Unlit = 0,
            UnlitTextured,
            Lit,
            LitTextured
        };
        
        VisibleObject(GameObject* object);
        ~VisibleObject();
        
        void LightingEnabledChanged(Material* material);
        
        GameObject* object;
        
        Transform* transform;
        Mesh* mesh;
        Material* material;
        TextureComponent* texture;
        Colorable* color;
        Shader* shader;
        Orderable* orderable;
        Type type;
        float distanceToCamera;
        
        void Refresh();
    };
    
    class IVisibleObjectManipulator {
    public:
        virtual void UpdateFromObject(GameObject* object) = 0;
    };
}