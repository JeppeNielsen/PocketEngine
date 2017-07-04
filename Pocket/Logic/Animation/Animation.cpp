//
//  Animation.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Animation.hpp"

using namespace Pocket;

Animation::Target& Animation::CreateTarget(Pocket::GameObject *object) {
    for(auto& target : targets) {
        if (target.object.operator()() == object) {
            return target;
        }
    }
    targets.resize(targets.size() + 1);
    targets.back().object = object;
    return targets.back();
}

void Animation::AddNode(Pocket::GameObject *object, int componentId, const std::string &fieldName, float time) {
    auto& target = CreateTarget(object);
    
    auto typeinfo = object->GetComponentTypeInfo(componentId);
    
    auto& nodes = target.nodes[typeinfo.name];
    
    auto fieldInfo = typeinfo.GetField(fieldName);
    
    IFieldDataCollection* collection;
    const auto& it = nodes.find(fieldName);
    if (it == nodes.end()) {
        auto collectionPtr = fieldInfo->CreateDataCollection();
        collection = collectionPtr.get();
        nodes[fieldName] = std::move(collectionPtr);
    } else {
        collection = it->second.get();
    }
    
    collection->AddData(time, fieldInfo.get());
}

void Animation::Apply(float time) {
    for(auto& target : targets) {
        GameObject* object = target.object();
        
        for(auto& node : target.nodes) {
            
            int index;
            if (object->World()->TryGetComponentIndex(node.first, index)) {
                TypeInfo info = object->GetComponentTypeInfo(index);
                for(auto& fieldName : node.second) {
                    auto field = info.GetField(fieldName.first);
                    if (field) {
                        fieldName.second->Apply(time, field.get());
                    }
                }
            }
        }
    }
}


