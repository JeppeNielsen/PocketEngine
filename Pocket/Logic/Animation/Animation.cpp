//
//  Animation.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Animation.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

Animation::Target& Animation::CreateTarget(Path& path) {
    for(auto& target : targets) {
        if (target.path == path) {
            return target;
        }
    }
    targets.resize(targets.size() + 1);
    targets.back().path = path;
    return targets.back();
}

void Animation::Path::Set(Pocket::GameObject *root, Pocket::GameObject *child) {
    if (root == child) return;
    
    GameObject* current = child;
    
    while (true) {
        indices.push_back(current->Hierarchy().ChildIndex());
        current = current->Hierarchy().Parent;
        if (!current || current == root) break;
    }
    std::reverse(indices.begin(), indices.end());
}

GameObject* Animation::Path::TryFindObject(Pocket::GameObject *root) {
    if (indices.empty()) return root;
    
    GameObject* parent = root;
    for (int i=0; i<indices.size(); i++) {
        auto& children = parent->Hierarchy().Children();
        int index = indices[i];
        if (index>=children.size()) {
            return 0;
        }
        parent = children[index];
    }
    return parent;
}

Animation::Target::Target(const Animation::Target& other) {
    path = other.path;
    components = other.components;
    for(auto& c : other.components) {
        auto& cc = components[c.first];
        for(auto& f : c.second) {
            if (f.second) {
                cc[f.first] = f.second->Clone();
            }
        }
    }
}

Animation::Target& Animation::Target::operator=(const Pocket::Animation::Target &other) {
    path = other.path;
    components = other.components;
    for(auto& c : other.components) {
        auto& cc = components[c.first];
        for(auto& f : c.second) {
            cc[f.first] = f.second->Clone();
        }
    }
    return *this;
}

void Animation::AddNode(GameObject *rootNode, GameObject* node, int componentId, const std::string &fieldName, float time) {

    Path path;
    path.Set(rootNode, node);

    auto& target = CreateTarget(path);
    
    auto typeinfo = node->GetComponentTypeInfo(componentId);
    
    auto& fieldTimelines = target.components[typeinfo.name];
    
    auto fieldInfo = typeinfo.GetField(fieldName);
    
    IFieldInfoTimeline* timeline;
    const auto& it = fieldTimelines.find(fieldName);
    if (it == fieldTimelines.end()) {
        auto timelinePtr = fieldInfo->CreateTimeline();
        timeline = timelinePtr.get();

        fieldTimelines[fieldName] = std::move(timelinePtr);
    } else {
        timeline = it->second.get();
    }
    
    timeline->AddData(time, fieldInfo.get());
}

void Animation::Apply(GameObject* root, float time) {
    for(auto& target : targets) {
        GameObject* object = target.path.TryFindObject(root);
        if (!object) continue;
        
        for(auto& componentTimeline : target.components) {
            
            int index;
            if (object->World()->Storage().TryGetComponentIndex(componentTimeline.first, index)) {
                TypeInfo info = object->GetComponentTypeInfo(index);
                for(auto& fieldTimeline : componentTimeline.second) {
                    if (!fieldTimeline.second) continue;
                    
                    auto field = info.GetField(fieldTimeline.first);
                    if (field) {
                        fieldTimeline.second->Apply(time, field.get());
                    }
                }
            }
        }
    }
}

const Animation::Targets& Animation::GetTargets() const { return targets; }

