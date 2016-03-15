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

void ColorSystem::ObjectAdded(Pocket::GameObject *object) {
    Colorable* colorable = object->GetComponent<Colorable>();
    colorable->Color.Changed.Bind(this, &ColorSystem::ColorChanged, object);
    object->GetComponent<Mesh>()->LocalBoundingBox.HasBecomeDirty.Bind(this, &ColorSystem::ColorChanged, object);
    ColorChanged(object);
}

void ColorSystem::ObjectRemoved(Pocket::GameObject *object) {
    Colorable* colorable = object->GetComponent<Colorable>();
    colorable->Color.Changed.Unbind(this, &ColorSystem::ColorChanged, object);
    object->GetComponent<Mesh>()->LocalBoundingBox.HasBecomeDirty.Unbind(this, &ColorSystem::ColorChanged, object);
    auto it = changedColorables.find(object);
    if (it != changedColorables.end()) {
        changedColorables.erase(it);
    }
}

void ColorSystem::ColorChanged(GameObject* object) {
    changedColorables.insert(object);
}

void ColorSystem::Update(float dt) {
    if (!changedColorables.empty()) {
        for (auto changedColorable : changedColorables) {
            Colorable* colorable = changedColorable->GetComponent<Colorable>();
            const Colour& color = colorable->Color;
            auto& verts = changedColorable->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
            
            for (size_t j=0; j<verts.size(); ++j) {
                verts[j].Color = color;
            }
        }
        changedColorables.clear();
    }
}

int ColorSystem::Order() {
    return 1000;
}