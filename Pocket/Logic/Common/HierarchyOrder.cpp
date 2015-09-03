//
//  HierarchyOrder.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "HierarchyOrder.hpp"
#include <algorithm>

using namespace Pocket;

void HierarchyOrder::Initialize() {
    AddComponent<Orderable>();
    orderIsDirty = false;
}

void HierarchyOrder::ObjectAdded(Pocket::GameObject *object) {
    object->Parent.ChangedWithOld += event_handler(this, &HierarchyOrder::ParentChanged);
    object->Order.Changed += event_handler(this, &HierarchyOrder::OrderChanged);
    orderIsDirty = true;
}

void HierarchyOrder::ObjectRemoved(Pocket::GameObject *object) {
   object->Order.Changed -= event_handler(this, &HierarchyOrder::OrderChanged);
}

void HierarchyOrder::ParentChanged(Property<GameObject*, GameObject*>::EventData d) {
    orderIsDirty = true;
}

void HierarchyOrder::OrderChanged(Pocket::GameObject *object) {
    orderIsDirty = true;
}

void HierarchyOrder::Update(float dt) {
    if (!orderIsDirty) return;
    orderIsDirty = false;

    int order = 0;
    for (ObjectCollection::const_iterator it = World()->Children().begin(); it!= World()->Children().end(); ++it) {
        CalculateOrder(order, *it);
    }
}

void HierarchyOrder::CalculateOrder(int& orderOffset, Pocket::GameObject *object) {
    
    Orderable* orderable = object->GetComponent<Orderable>();
    if (orderable) orderable->Order = orderOffset + object->Order;
    
    const ObjectCollection& children = object->Children();
    if (children.empty()) return;
    
    int minOrder = children[0]->Order;
    int maxOrder = children[0]->Order;
    
    for (ObjectCollection::const_iterator it = children.begin(); it!=children.end(); ++it) {
        int order = (*it)->Order;
        if (order>maxOrder) {
            maxOrder = order;
        }
        if (order<minOrder) {
            minOrder = order;
        }
    }
    
    orderOffset += (maxOrder - minOrder) + 1;
    for (ObjectCollection::const_iterator it = children.begin(); it!=children.end(); ++it) {
        CalculateOrder(orderOffset, *it);
    }
}
