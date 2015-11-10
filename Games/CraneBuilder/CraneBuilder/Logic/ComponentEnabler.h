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

Component(ComponentEnabler)

    struct EnabledComponent : public ISerializable {
        std::string id;
        int component;
        SERIALIZE_FIELDS_BEGIN
        SERIALIZE_FIELD(id);
        SERIALIZE_FIELD(component);
        SERIALIZE_FIELDS_END
    };

    typedef std::vector<EnabledComponent> EnabledComponents;
    EnabledComponents components;

    template<class T>
    void AddComponent(const std::string& id) {
        EnabledComponent e;
        e.id = id;
        e.component = T::ID;
        components.push_back(e);
    }

    SERIALIZE_FIELDS_BEGIN
    SERIALIZE_FIELD(components)
    SERIALIZE_FIELDS_END
};