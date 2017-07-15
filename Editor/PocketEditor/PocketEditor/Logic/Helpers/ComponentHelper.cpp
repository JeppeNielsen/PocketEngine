//
//  ComponentHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/05/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ComponentHelper.hpp"
#include <algorithm>
#include "ScriptWorld.hpp"

ComponentHelper::ComponentCollection ComponentHelper::GetSortedComponents(Pocket::GameWorld &world) {

    using namespace std;
    
    const static map<string, vector<string>> categories = {
        { "Animation", {
            "Pocket::Animation",
            "Pocket::Animator",
        }},
        { "Audio", {
            "Pocket::Sound",
            "Pocket::SoundEmitter",
            "Pocket::SoundListener",
        }},
        { "Cloning", {
            "Pocket::Cloner",
            "Pocket::CloneVariable",
        }},
        { "Common", {
            "Pocket::Orderable",
        }},
        { "Effects", {
            "Pocket::ParticleEffect",
            "Pocket::ParticleEmitter",
        }},
        { "Gui", {
            "Pocket::Droppable",
            "Pocket::Font",
            "Pocket::Label",
            "Pocket::Layouter",
            "Pocket::SlicedQuad",
            "Pocket::Sizeable",
            "Pocket::Sprite",
            "Pocket::TextBox",
        }},
        { "Input", {
            "Pocket::InputController",
            "Pocket::InputMapper",
        }},
        { "Interaction", {
            "Pocket::Touchable",
            "Pocket::TouchableCaneller",
        }},
        { "Movement", {
            "Pocket::Draggable",
            "Pocket::DraggableMotion",
            "Pocket::FirstPersonMover",
            "Pocket::Limitable",
            "Pocket::Velocity",
        }},
        { "Physics", {
            "Pocket::Collidable",
            "Pocket::Joint2d",
            "Pocket::RigidBody",
            "Pocket::RigidBody2d",
        }},
        { "Rendering", {
            "Pocket::Camera",
            "Pocket::Colorable",
            "Pocket::LineRenderer",
            "Pocket::Mesh",
            "Pocket::Renderable",
            "Pocket::ShaderComponent",
            "Pocket::SlicedTexture",
            "Pocket::TextureComponent",
        }},
        { "Scenes", {
            "Pocket::SceneManager",
        }},
        { "Selection", {
            "Pocket::Selectable",
            "Pocket::SelectedColorer",
        }},
        { "Spatial", {
            "Pocket::Transform",
        }},
        { "Spawning", {
            "Pocket::Spawner",
        }},
        { "Triggering", {
            "Pocket::Trigger",
        }},
    };
    
    ComponentCollection components;
    
    for(auto& category : categories) {
        std::vector<ComponentInfo> componentInfos;
        for(auto& component : category.second) {
            int index;
            if (world.TryGetComponentIndex(component, index)) {
                componentInfos.push_back({ component, index, category.first });
            }
        }
        
        if (!componentInfos.empty()) {
            components[category.first] = componentInfos;
        }
    }
    return components;
}

ComponentHelper::ComponentCollection ComponentHelper::GetSortedScriptComponents(Pocket::GameWorld &world, Pocket::ScriptWorld& scriptWorld) {

    using namespace std;

    int componentCount = scriptWorld.ComponentCount();
    auto componentTypes = world.GetComponentTypes();
    
    int startIndex = (int)componentTypes.size() - componentCount;
    
    ComponentCollection components;
    
    for (int i=startIndex; i<componentTypes.size(); ++i) {
        auto& ct = componentTypes[i];
        auto namespaceIndex = ct.name.find("::");
        if (namespaceIndex == std::string::npos){
           components["Global"].push_back({ componentTypes[i].name, i, "Global" });
        } else {
            std::string nameSpace = ct.name.substr(0, namespaceIndex);
            std::string nameWithoutNamespace = ct.name.substr(namespaceIndex+2, ct.name.size() - 2 - namespaceIndex);
            components[nameSpace].push_back({ nameWithoutNamespace, i, nameSpace });
        }
    }
    
    return components;
}
