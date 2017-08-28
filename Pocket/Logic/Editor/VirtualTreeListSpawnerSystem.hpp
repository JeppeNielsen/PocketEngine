//
//  VirtualTreeListSpawnerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "VirtualTreeList.hpp"
#include "VirtualTreeListSpawner.hpp"
#include "Touchable.hpp"
#include "Sizeable.hpp"

namespace Pocket {
  class VirtualTreeListSpawnerSystem : public GameSystem<VirtualTreeList, VirtualTreeListSpawner, Transform, Sizeable> {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        void NodeCreated(VirtualTreeList::Node e, GameObject* object);
        void NodeRemoved(VirtualTreeList::Node e, GameObject* object);
        void NodeExpanded(VirtualTreeList::Node e, GameObject* object);
      
        struct FoldoutData {
            GameObject* node;
            VirtualTreeList* treelist;
            
            inline bool operator ==(const FoldoutData &other) const{
                return !(node!=other.node || treelist!=other.treelist);
            }

            inline bool operator !=(const FoldoutData &other) const{
                return (node!=other.node || treelist!=other.treelist);
            }
        };
      
        void FoldOutClicked(TouchData d, FoldoutData foldout);
        void SizeChanged(GameObject* object);
      
  };
}
