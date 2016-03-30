//
//  VirtualTreeListSpawnerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "VirtualTreeListSpawnerSystem.hpp"
#include "Colorable.hpp"

using namespace Pocket;

void VirtualTreeListSpawnerSystem::Initialize() {

}

void VirtualTreeListSpawnerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<VirtualTreeList>()->NodeCreated.Bind(this, &VirtualTreeListSpawnerSystem::NodeCreated, object);
    object->GetComponent<VirtualTreeList>()->NodeRemoved.Bind(this, &VirtualTreeListSpawnerSystem::NodeRemoved, object);
}


void VirtualTreeListSpawnerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<VirtualTreeList>()->NodeCreated.Unbind(this, &VirtualTreeListSpawnerSystem::NodeCreated, object);
    object->GetComponent<VirtualTreeList>()->NodeRemoved.Unbind(this, &VirtualTreeListSpawnerSystem::NodeRemoved, object);
    
    for(auto& o : object->GetComponent<VirtualTreeListSpawner>()->objects) {
        o.data->Remove();
    }
    object->GetComponent<VirtualTreeListSpawner>()->objects.clear();
}

void VirtualTreeListSpawnerSystem::NodeCreated(VirtualTreeList::Node e, Pocket::GameObject *object) {
    VirtualTreeListSpawner* spawner = object->GetComponent<VirtualTreeListSpawner>();
    auto& objects = spawner->objects;
    GameObject* clone = spawner->prefab->Clone(object);
    clone->GetComponent<Transform>()->Position = {e.depth*25.0f,e.position * 25.0f,0};
    objects.push_back({ e.node, e.position, e.depth, clone });
    
    VirtualTreeList* treeList = object->GetComponent<VirtualTreeList>();
    
    clone->Children()[0]->GetComponent<Touchable>()->Click.Bind(this, &VirtualTreeListSpawnerSystem::FoldOutClicked, {e.node, treeList });
    
    clone->Children()[0]->GetComponent<Colorable>()->Color = e.node->Children().empty() ? Colour::White() : Colour::Red();
    
    //std::cout << "Created object : " << e.node->ID<<" index : "<<e.position<< "depth : " << e.depth << std::endl;
}

void VirtualTreeListSpawnerSystem::NodeRemoved(VirtualTreeList::Node e, Pocket::GameObject *object) {
    VirtualTreeListSpawner* spawner = object->GetComponent<VirtualTreeListSpawner>();
    auto& objects = spawner->objects;
    auto it = std::find(objects.begin(), objects.end(), e);
    GameObject* clone = it->data;
    clone->Remove();
    objects.erase(it);
    clone->Children()[0]->GetComponent<Touchable>()->Click.Unbind(this, &VirtualTreeListSpawnerSystem::FoldOutClicked, {e.node, object->GetComponent<VirtualTreeList>()});
    
    //std::cout << "Removed object : " << e.node->ID << std::endl;
}

void VirtualTreeListSpawnerSystem::FoldOutClicked(Pocket::TouchData d, FoldoutData foldout) {
    foldout.treelist->SetNodeExpanded(foldout.node, !foldout.treelist->IsNodeExpanded(foldout.node));
}
