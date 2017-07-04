//
//  Animation.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TypeInfo.hpp"
#include <map>
#include <memory>
#include "GameObjectHandle.hpp"

namespace Pocket {
    class Animation {
    public:
        
        using Nodes = std::map<std::string, std::shared_ptr<IFieldDataCollection>>;
        Nodes nodes;
        
        struct Target {
            GameObjectHandle object;
            std::map<std::string, Nodes> nodes;
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(object)
            TYPE_FIELD(nodes)
            TYPE_FIELDS_END
        };
    
        using Targets = std::vector<Target>;
        Targets targets;
        
        Target& CreateTarget(GameObject* object);
        
    public:
        void AddNode(GameObject* object, int componentId, const std::string& fieldName, float time);
        void Apply(float time);
        
        template<typename T>
        void AddNode(GameObject* object, const std::string& fieldName, float time) {
            AddNode(object, GameIdHelper::GetComponentID<T>(), fieldName, time);
        }
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(targets)
        TYPE_FIELDS_END
        
    };
}
