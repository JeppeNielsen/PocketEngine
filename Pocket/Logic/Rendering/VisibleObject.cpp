//
//  VisibleObject.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "VisibleObject.hpp"

using namespace Pocket;

VisibleObject::VisibleObject(GameObject* object)
:
object(object)
{
    transform = object->GetComponent<Transform>();
    mesh = object->GetComponent<Mesh>();
    material = object->GetComponent<Material>();
    if (material) {
        material->LightingEnabled.Changed += event_handler(this, &VisibleObject::LightingEnabledChanged);
    }
    shader = 0;
    Refresh();
}

VisibleObject::~VisibleObject() {
    if (material) {
        material->LightingEnabled.Changed -= event_handler(this, &VisibleObject::LightingEnabledChanged);
    }
}

void VisibleObject::LightingEnabledChanged(Pocket::Material *material) {
    Refresh();
}

void VisibleObject::Refresh() {
    
    texture = object->IsComponentEnabled<TextureComponent>() ? object->GetComponent<TextureComponent>() : 0;
    color = object->IsComponentEnabled<Colorable>() ? object->GetComponent<Colorable>() : 0;
    
    bool useLighting = material ? material->LightingEnabled : false;
    
    if (!useLighting) {
        type = texture ? UnlitTextured : Unlit;
    } else {
        type = texture ? LitTextured : Lit;
    }
    
    orderable = object->GetComponent<Orderable>();
    
    
    //UnlitUnColored
    //UnlitColored
    //UnlitUnColoredTextured
    //UnlitColoredTextured
    
    //LitUnColored
    //LitColored
    //LitUnColoredTextured
    //LitColoredTextured
    
}
