//
//  OctreeSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/20/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "OctreeSystem.hpp"

using namespace Pocket;

void OctreeSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Mesh>();
    SetWorldBounds(BoundingBox(0, 1000));
}

void OctreeSystem::SetWorldBounds(const Pocket::BoundingBox &bounds) {
    octree.SetBoundingBox(bounds);
}

void OctreeSystem::ObjectAdded(GameObject* object) {
    SetMetaData(object, new Node(object, object->GetComponent<Transform>(), object->GetComponent<Mesh>(), this));
}

void OctreeSystem::ObjectRemoved(GameObject* object) {
    Node* node = (Node*)GetMetaData(object);
    delete node;
}

void OctreeSystem::UpdateAllNodes() {
    for(unsigned i=0; i<octreeObjectsUpdateList.size(); i++) {
        Node* node = octreeObjectsUpdateList[i];
        if (node) node->UpdateOctreeNode();
	}
	octreeObjectsUpdateList.clear();
}

void OctreeSystem::GetObjectsInFrustum(const Pocket::BoundingFrustum &frustum, ObjectCollection &objectList) {
    UpdateAllNodes();
    octree.Get<GameObject*>(frustum, objectList);
}

void OctreeSystem::GetObjectsAtRay(const Pocket::Ray &ray, ObjectCollection &objectList) {
    UpdateAllNodes();
    octree.Get<GameObject*>(ray, objectList);
}

OctreeSystem::Node::Node(GameObject* object, Transform* transform, Mesh* mesh, OctreeSystem* system) :
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
    
	transform->World.HasBecomeDirty += event_handler(this, &OctreeSystem::Node::TransformChanged);
    mesh->LocalBoundingBox.HasBecomeDirty += event_handler(this, &OctreeSystem::Node::MeshBoundingBoxChanged);
    
    SetOctreeNodeDirty();
}

OctreeSystem::Node::~Node() {
    
	transform->World.HasBecomeDirty -= event_handler(this, &OctreeSystem::Node::TransformChanged);
    mesh->LocalBoundingBox.HasBecomeDirty -= event_handler(this, &OctreeSystem::Node::MeshBoundingBoxChanged);
    
    if (octreeNodeDirty) {
        system->octreeObjectsUpdateList[indexInList] = 0;
    }
    
    if (octreeNode.node) {
        system->octree.Remove(octreeNode);
    }
}

void OctreeSystem::Node::TransformChanged(DirtyProperty<Transform*, Matrix4x4>* transform) {
	boundingBoxDirty = true;
	SetOctreeNodeDirty();
}

void OctreeSystem::Node::MeshBoundingBoxChanged(DirtyProperty<Mesh*, BoundingBox>* mesh) {
	boundingBoxDirty = true;
	SetOctreeNodeDirty();
}

bool OctreeSystem::Node::InView(const BoundingFrustum& frustum) {
	if (boundingBoxDirty) {
		boundingBoxDirty = false;
		BoundingBox* box = mesh->LocalBoundingBox.GetValue();
		box->CreateWorldAligned(*transform->World.GetValue(), boundingBox);
	}
	return frustum.Intersect(boundingBox) != BoundingFrustum::OUTSIDE;
}

void OctreeSystem::Node::SetOctreeNodeDirty() {
	if (octreeNodeDirty) return;
	octreeNodeDirty = true;
    indexInList = system->octreeObjectsUpdateList.size();
	system->octreeObjectsUpdateList.push_back(this);
}

void OctreeSystem::Node::UpdateOctreeNode() {
    
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




