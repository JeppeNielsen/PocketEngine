//
//  HierarchyOrder.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "HierarchyOrder.hpp"

using namespace Pocket;

void HierarchyOrder::ObjectAdded(GameObject *object) {
    object->Hierarchy().Parent.Changed.Bind(this, &HierarchyOrder::SetDirty);
    object->Hierarchy().Order.Changed.Bind(this, &HierarchyOrder::SetDirty);
    orderIsDirty = true;
}

void HierarchyOrder::ObjectRemoved(GameObject *object) {
   object->Hierarchy().Parent.Changed.Unbind(this, &HierarchyOrder::SetDirty);
   object->Hierarchy().Order.Changed.Unbind(this, &HierarchyOrder::SetDirty);
}

void HierarchyOrder::SetDirty() {
    orderIsDirty = true;
}

void HierarchyOrder::Update(float dt) {
    if (!orderIsDirty) return;
    orderIsDirty = false;

    int order = 0;
    CalculateOrder(order, root);
}

void HierarchyOrder::CalculateOrder(int& orderOffset, GameObject *object) {
    
    orderOffset++;
    
    Orderable* orderable = object->GetComponent<Orderable>();
    if (orderable) orderable->Order = orderOffset;
    
    const auto& children = object->Hierarchy().Children();
    if (children.empty()) return;
    
    auto sortedChildren = children;
    std::sort(sortedChildren.begin(), sortedChildren.end(), [] (GameObject* a, GameObject* b) {
        return a->Hierarchy().Order()<b->Hierarchy().Order();
    });
    
    for(auto child : sortedChildren) {
        CalculateOrder(orderOffset, child);
    }
}
