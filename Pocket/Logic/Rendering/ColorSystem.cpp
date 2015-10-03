//
//  ColorSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ColorSystem.hpp"
#include "Vertex.hpp"
#include <algorithm>

using namespace Pocket;

void ColorSystem::Initialize() {
    AddComponent<Mesh>();
    AddComponent<Colorable>();
}

void ColorSystem::ObjectAdded(Pocket::GameObject *object) {
    Colorable* colorable = object->GetComponent<Colorable>();
    colorable->Color.Changed += event_handler(this, &ColorSystem::ColorChanged, object);
    ColorChanged(colorable, object);
}

void ColorSystem::ObjectRemoved(Pocket::GameObject *object) {
    Colorable* colorable = object->GetComponent<Colorable>();
    colorable->Color.Changed -= event_handler(this, &ColorSystem::ColorChanged, object);
    auto it = changedColorables.find(object);
    if (it != changedColorables.end()) {
        changedColorables.erase(it);
    }
}

void ColorSystem::ColorChanged(Pocket::Colorable *colorable, GameObject* object) {
    changedColorables.insert(object);
}

void ColorSystem::Update(float dt) {
    if (!changedColorables.empty()) {
        for (auto changedColorable : changedColorables) {
            Colorable* colorable = changedColorable->GetComponent<Colorable>();
            const Colour& color = colorable->Color.GetValue();
            auto& verts = changedColorable->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
            
            for (size_t j=0; j<verts.size(); ++j) {
                verts[j].Color = color;
            }
        }
        changedColorables.clear();
    }
}
