//
//  Panel.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "PanelLocation.hpp"
#include "Property.hpp"

namespace Pocket {
    class GameObject;

    class Panel {
    public:
        Panel();
        
        Property<GameObject*> Area;
        PanelLocation location;
        Event<> Dirty;
    };
}
