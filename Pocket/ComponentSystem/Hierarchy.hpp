//
//  Hierarchy.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 05/01/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    
    class GameObject;
    
    class Hierarchy {
    public:
        Hierarchy();
        Hierarchy(const Hierarchy& other);
        
        using ChildrenCollection = std::vector<GameObject*>;
    private:
        ChildrenCollection children;
        int childIndex;
        GameObject* owner;
        bool worldEnabled;
        
        void CalculateEnabled(GameObject* parent, GameObject* object);
        
    public:
        Property<GameObject*> Parent;
        const ChildrenCollection& Children();
        Property<bool> Enabled;
        Property<int> Order;
        bool WorldEnabled() const;
        Event<> WorldEnabledChanged;
        int ChildIndex() const;
        
        friend class GameObject;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Enabled)
        TYPE_FIELDS_END
    };
}
