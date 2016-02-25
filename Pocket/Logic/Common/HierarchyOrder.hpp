//
//  HierarchyOrder.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Orderable.hpp"
#include <set>

namespace Pocket {
    template<typename T>
    class HierarchyOrder : public GameSystem<T, Orderable> {
    public:
        using GameObject = GameObject<T>;
        
        void ObjectAdded(GameObject *object) {
            object->Parent.Changed.Bind(this, &HierarchyOrder::SetDirty);
            object->Order.Changed.Bind(this, &HierarchyOrder::SetDirty);
            orderIsDirty = true;
        }

        void ObjectRemoved(GameObject *object) {
           object->Parent.Changed.Unbind(this, &HierarchyOrder::SetDirty);
           object->Order.Changed.Unbind(this, &HierarchyOrder::SetDirty);
        }

        void SetDirty() {
            orderIsDirty = true;
        }

        void Update(float dt) {
            if (!orderIsDirty) return;
            orderIsDirty = false;

            int order = 0;
            for (int i=0; i<this->World().ObjectCount(); ++i) {
                GameObject* object = this->World().GetObject(i);
                if (object->Parent) continue;
                CalculateOrder(order, object);
            }
        }

        void CalculateOrder(int& orderOffset, GameObject *object) {
            
            orderOffset++;
            
            Orderable* orderable = object->template GetComponent<Orderable>();
            if (orderable) orderable->Order = orderOffset;
            
            const auto& children = object->Children();
            if (children.empty()) return;
            
            auto sortedChildren = children;
            std::sort(sortedChildren.begin(), sortedChildren.end(), [] (GameObject* a, GameObject* b) {
                return a->Order()<b->Order();
            });
            
            for(auto child : sortedChildren) {
                CalculateOrder(orderOffset, child);
            }
        }
        
    private:
        bool orderIsDirty;
    };
}