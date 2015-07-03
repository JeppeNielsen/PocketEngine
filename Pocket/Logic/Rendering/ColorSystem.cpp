//
//  ColorSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ColorSystem.hpp"
#include "Vertex.hpp"

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
    std::vector<GameObject*>::iterator it = std::find(changedColorables.begin(), changedColorables.end(), object);
    if (it != changedColorables.end()) {
        changedColorables.erase(it);
    }
}

void ColorSystem::ColorChanged(Pocket::Colorable *colorable, GameObject* object) {
    changedColorables.push_back(object);
}

void ColorSystem::Update(float dt) {
    if (!changedColorables.empty()) {
        for (size_t i = 0; i<changedColorables.size(); i++) {
            Colorable* colorable = changedColorables[i]->GetComponent<Colorable>();
            const Colour& color = colorable->Color.GetValue();
            auto& verts = changedColorables[i]->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
            
            for (size_t j=0; j<verts.size(); ++j) {
                verts[j].Color = color;
            }
        }
        changedColorables.clear();
    }
}
