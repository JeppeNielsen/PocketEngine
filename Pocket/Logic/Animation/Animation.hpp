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
        using FieldTimelines = std::map<std::string, std::shared_ptr<IFieldInfoTimeline>>;
        
        struct Path {
            std::vector<int> indices;
            void Set(GameObject* root, GameObject* child);
        
            inline bool operator ==(const Path& other) const{
                if (other.indices.size()!=indices.size()) return false;
                for(int i=0; i<indices.size(); ++i) {
                    if (indices[i]!=other.indices[i]) return false;
                }
                return true;
            }
        
            inline bool operator !=(const Path& other) const{
                return !operator==(other);
            }
            
            GameObject* TryFindObject(GameObject* root);
        };
        
        struct Target {
            Path path;
            std::map<std::string, FieldTimelines> components;
            
            Target() = default;
            Target(const Target& target);
            Target& operator=(const Target& other);
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(path.indices)
            TYPE_FIELD(components)
            TYPE_FIELDS_END
        };
    
        using Targets = std::vector<Target>;
    private:
        Targets targets;
        Target& CreateTarget(Path& path);
        
    public:
    
        void AddNode(GameObject* rootNode, GameObject* node, int componentId, const std::string& fieldName, float time);
        void Apply(GameObject* root, float time);
        
        template<typename T>
        void AddNode(GameObject* rootNode, GameObject* node, const std::string& fieldName, float time) {
            AddNode(rootNode, node, GameIdHelper::GetComponentID<T>(), fieldName, time);
        }
        
        const Targets& GetTargets() const;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(targets)
        TYPE_FIELDS_END
    };
}
