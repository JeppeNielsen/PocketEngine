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
    object->OrderChanged += event_handler(this, &HierarchyOrder::OrderChanged);
    orderIsDirty = true;
}

void HierarchyOrder::ObjectRemoved(Pocket::GameObject *object) {
   object->OrderChanged -= event_handler(this, &HierarchyOrder::OrderChanged);
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

void HierarchyOrder::CalculateOrder(int &order, Pocket::GameObject *object) {
    
    Orderable* orderable = object->GetComponent<Orderable>();
    if (orderable) orderable->Order = order;
    
    order++;
    
    const ObjectCollection& children = object->Children();
    
    for (ObjectCollection::const_iterator it = children.begin(); it!=children.end(); ++it) {
        CalculateOrder(order, *it);
    }
}
