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
        o.second.parent->Remove();
    }
    object->GetComponent<VirtualTreeListSpawner>()->objects.clear();
}

void VirtualTreeListSpawnerSystem::NodeCreated(VirtualTreeList::Node e, Pocket::GameObject *object) {
    VirtualTreeList* treeList = object->GetComponent<VirtualTreeList>();
    VirtualTreeListSpawner* spawner = object->GetComponent<VirtualTreeListSpawner>();
    auto& objects = spawner->objects;
    
    Vector3 nodePosition = {e.depth*treeList->ItemIndent(),e.position * -treeList->ItemHeight(),0};
    
    VirtualTreeListSpawner::SpawnedNode node;
    node.node = e.node;
    node.parent = object->CreateChild();
    node.parent->AddComponent<Transform>()->Position = nodePosition;
    node.height = treeList->ItemHeight;
    node.foldOutButton = 0;
    node.hasChildren = !spawner->HasChildren ? !e.node->Children().empty() : spawner->HasChildren(e.node);
    
    if (spawner->OnCreate) {
        spawner->OnCreate(node);
    }
    objects[e] = node;
    
    if (node.foldOutButton && node.foldOutButton->HasComponent<Touchable>()) {
        node.foldOutButton->GetComponent<Touchable>()->Click.Bind(this, &VirtualTreeListSpawnerSystem::FoldOutClicked, {e.node, treeList });
    }
}

void VirtualTreeListSpawnerSystem::NodeRemoved(VirtualTreeList::Node e, Pocket::GameObject *object) {
    VirtualTreeListSpawner* spawner = object->GetComponent<VirtualTreeListSpawner>();
    auto& objects = spawner->objects;
    
    auto it = objects.find(e);
    auto& node = it->second;
    
    if (spawner->OnRemove) {
        spawner->OnRemove(node);
    }
    node.parent->Remove();
    objects.erase(it);
    
    if (node.foldOutButton && node.foldOutButton->HasComponent<Touchable>()) {
        node.foldOutButton->GetComponent<Touchable>()->Click.Unbind(this, &VirtualTreeListSpawnerSystem::FoldOutClicked, {e.node, object->GetComponent<VirtualTreeList>()});
    }
    //std::cout << "Removed object : " << e.node->ID << std::endl;
}

void VirtualTreeListSpawnerSystem::FoldOutClicked(Pocket::TouchData d, FoldoutData foldout) {
    foldout.treelist->SetNodeExpanded(foldout.node, !foldout.treelist->IsNodeExpanded(foldout.node));
}
