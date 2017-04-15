//
//  VirtualTreeListSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "VirtualTreeListSystem.hpp"
#include <cmath>


using namespace Pocket;

void VirtualTreeListSystem::Initialize() {

}

void VirtualTreeListSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<VirtualTreeList>()->Root.Changed.Bind(this, &VirtualTreeListSystem::UpdateVirtualList, object);
}

void VirtualTreeListSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<VirtualTreeList>()->Root.Changed.Unbind(this, &VirtualTreeListSystem::UpdateVirtualList, object);
}

void VirtualTreeListSystem::Update(float dt) {
    for(auto o : Objects()) {
        UpdateVirtualList(o);
    }
}

void VirtualTreeListSystem::UpdateVirtualList(Pocket::GameObject *object) {
    VirtualTreeList* treeList = object->GetComponent<VirtualTreeList>();
    if (!treeList->Pivot()) return;
    if (!treeList->Root()) {
        treeList->Clear();
        return;
    }
    
    Transform* pivotTransform = treeList->Pivot()->GetComponent<Transform>();
    if (!pivotTransform) return;
    
    Transform* transform = object->GetComponent<Transform>();
    Sizeable* sizable = treeList->Pivot()->GetComponent<Sizeable>();
    
    Vector3 startWorldPosition = pivotTransform->World().TransformPosition(0);
    Vector3 startPosition = transform->WorldInverse().TransformPosition(startWorldPosition);
    
    Vector3 endWorldPosition = pivotTransform->World().TransformPosition({0, sizable->Size().y,0});
    Vector3 endPosition = transform->WorldInverse().TransformPosition(endWorldPosition);
    
    float itemHeight = treeList->ItemHeight;

    //int startIndex = (int)floorf(startPosition.y / itemHeight);
    //int endIndex = (int)floorf(endPosition.y / itemHeight);

    int startIndex = (int)-floorf(endPosition.y / itemHeight);
    int endIndex = (int)-floorf(startPosition.y / itemHeight);


    VirtualTreeList::Nodes nodes;
    treeList->GetNodes(startIndex, endIndex, nodes);

    for(auto& node : treeList->visibleNodes) {
        auto current = std::find(nodes.begin(), nodes.end(), node);
        bool isRemoved = current == nodes.end();
        if (isRemoved) {
            treeList->NodeRemoved({ node.node, node.position, node.depth });
        }
    }

    for(auto& node : nodes) {
        auto prev = std::find(treeList->visibleNodes.begin(), treeList->visibleNodes.end(), node);
        bool isCreated = prev == treeList->visibleNodes.end();
        if (isCreated) {
            treeList->NodeCreated({ node.node, node.position, node.depth });
        }
    }

    

    //std::cout << "startIndex : " << startIndex << "   endIndex : " << endIndex<< std::endl;

    treeList->visibleNodes =  nodes;
}
