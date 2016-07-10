//
//  ComponentEnabler.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 10/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

using namespace Pocket;

struct ComponentEnabler {

    struct EnabledComponent {
        std::string id;
        int component;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(id);
        TYPE_FIELD(component);
        TYPE_FIELDS_END
    };

    typedef std::vector<EnabledComponent> EnabledComponents;
    EnabledComponents components;

    template<class T>
    void AddComponent(const std::string& id) {
        EnabledComponent e;
        e.id = id;
        e.component =  GameIDHelper::GetComponentID<T>();
        components.push_back(e);
    }

    TYPE_FIELDS_BEGIN
    TYPE_FIELD(components)
    TYPE_FIELDS_END
};