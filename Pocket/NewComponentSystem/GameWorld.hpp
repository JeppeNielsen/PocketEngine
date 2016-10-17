//
//  GameWorld.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>
#include "Container.hpp"
#include "TypeInfo.hpp"
#include "GameIdHelper.hpp"
#include "GameObject.hpp"

namespace Pocket {
    class GameWorld {
    private:
    
        struct ComponentInfo {
            ComponentInfo() : container(0) {}
            IContainer* container;
            std::string name;
            std::function<TypeInfo(GameObject*)> getTypeInfo;
        };
    
        using Components = std::vector<ComponentInfo>;
        Components components;
        
        
        
        
    private:
        void AddComponentType(ComponentId componentId, const std::function<void(ComponentInfo& componentInfo)>& constructor);
        
        template<typename T>
        void AddComponentType() {
            TryAddComponentType(GameIdHelper::GetComponentID<T>(), [] (ComponentInfo& componentInfo) {
                componentInfo.container = new Container<T>();
                componentInfo.name = GameIdHelper::GetClassName<T>();
                componentInfo.getTypeInfo = [] (GameObject* object) {
                    T* component = object->GetComponent<T>();
                    return component->GetType();
                };
            });
        }
        
        
        
        
    };
}