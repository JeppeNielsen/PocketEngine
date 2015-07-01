//
//  MeshOctreeSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/20/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "MeshOctreeSystem.h"

void MeshOctreeSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<MeshComponent>();
    SetWorldBounds(BoundingBox(0, 1000));
}

void MeshOctreeSystem::SetWorldBounds(const Pocket::BoundingBox &bounds) {
    octree.SetBoundingBox(bounds);
}

void MeshOctreeSystem::ObjectAdded(GameObject* object) {
    SetMetaData(object, new Node(object, object->GetComponent<Transform>(), object->GetComponent<MeshComponent>(), this));
}

void MeshOctreeSystem::ObjectRemoved(GameObject* object) {
    Node* node = (Node*)GetMetaData(object);
    delete node;
}

void MeshOctreeSystem::UpdateAllNodes() {
    for(unsigned i=0; i<octreeObjectsUpdateList.size(); i++) {
        Node* node = octreeObjectsUpdateList[i];
        if (node) node->UpdateOctreeNode();
	}
	octreeObjectsUpdateList.clear();
}

void MeshOctreeSystem::GetObjectsInFrustum(const Pocket::BoundingFrustum &frustum, ObjectCollection &objectList) {
    UpdateAllNodes();
    octree.Get<GameObject*>(frustum, objectList);
}

void MeshOctreeSystem::GetObjectsAtRay(const Pocket::Ray &ray, ObjectCollection &objectList) {
    UpdateAllNodes();
    octree.Get<GameObject*>(ray, objectList);
}

MeshOctreeSystem::Node::Node(GameObject* object, Transform* transform, MeshComponent* mesh, MeshOctreeSystem* system) :
transform(transform),
mesh(mesh),
system(system),
indexInList(0)
{
	boundingBoxDirty = true;
	octreeNodeDirty = false;
    octreeNode.order = 0;
	octreeNode.node = 0;
	octreeNode.data = object;
    
	transform->World.HasBecomeDirty += event_handler(this, &MeshOctreeSystem::Node::TransformChanged);
    mesh->LocalBoundingBox.HasBecomeDirty += event_handler(this, &MeshOctreeSystem::Node::MeshBoundingBoxChanged);
    
    SetOctreeNodeDirty();
}

MeshOctreeSystem::Node::~Node() {
    
	transform->World.HasBecomeDirty -= event_handler(this, &MeshOctreeSystem::Node::TransformChanged);
    mesh->LocalBoundingBox.HasBecomeDirty -= event_handler(this, &MeshOctreeSystem::Node::MeshBoundingBoxChanged);
    
    if (octreeNodeDirty) {
        system->octreeObjectsUpdateList[indexInList] = 0;
    }
    
    if (octreeNode.node) {
        system->octree.Remove(octreeNode);
    }
}

void MeshOctreeSystem::Node::TransformChanged(DirtyProperty<Transform*, Matrix4x4>* transform) {
	boundingBoxDirty = true;
	SetOctreeNodeDirty();
}

void MeshOctreeSystem::Node::MeshBoundingBoxChanged(DirtyProperty<MeshComponent*, BoundingBox>* mesh) {
	boundingBoxDirty = true;
	SetOctreeNodeDirty();
}

bool MeshOctreeSystem::Node::InView(const BoundingFrustum& frustum) {
	if (boundingBoxDirty) {
		boundingBoxDirty = false;
		BoundingBox* box = mesh->LocalBoundingBox.GetValue();
		box->CreateWorldAligned(*transform->World.GetValue(), boundingBox);
	}
	return frustum.Intersect(boundingBox) != BoundingFrustum::OUTSIDE;
}

void MeshOctreeSystem::Node::SetOctreeNodeDirty() {
	if (octreeNodeDirty) return;
	octreeNodeDirty = true;
    indexInList = system->octreeObjectsUpdateList.size();
	system->octreeObjectsUpdateList.push_back(this);
}

void MeshOctreeSystem::Node::UpdateOctreeNode() {
    
	octreeNodeDirty = false;
    
	if (boundingBoxDirty) {
		boundingBoxDirty = false;
		BoundingBox* box = mesh->LocalBoundingBox.GetValue();
		box->CreateWorldAligned(*transform->World.GetValue(), octreeNode.box);
	}
    
	if (!octreeNode.node) {
		system->octree.Insert(octreeNode);
	} else {
		system->octree.Move(octreeNode);
	}
}




