//
//  OctreeSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/20/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Octree.hpp"

namespace Pocket {
    template<typename...ExtraComponents>
    class OctreeSystem : public GameSystem<Transform, Mesh, ExtraComponents...> {
    public:
        
        OctreeSystem() {
            SetWorldBounds(BoundingBox(0, 1000));
        }

        void SetWorldBounds(const Pocket::BoundingBox &bounds) {
            octree.SetBoundingBox(bounds);
        }

        void ObjectAdded(GameObject* object) {
            this->SetMetaData(object, new Node(object, object->GetComponent<Transform>(), object->GetComponent<Mesh>(), this));
        }

        void ObjectRemoved(GameObject* object) {
            Node* node = (Node*)this->GetMetaData(object);
            delete node;
        }

        void UpdateAllNodes() {
            for(unsigned i=0; i<octreeObjectsUpdateList.size(); i++) {
                Node* node = octreeObjectsUpdateList[i];
                if (node) node->UpdateOctreeNode();
            }
            octreeObjectsUpdateList.clear();
        }

        void GetObjectsInFrustum(const Pocket::BoundingFrustum &frustum, ObjectCollection& objectList) {
            UpdateAllNodes();
            octree.Get<GameObject*>(frustum, objectList);
        }

        void GetObjectsAtRay(const Pocket::Ray &ray, ObjectCollection& objectList) {
            UpdateAllNodes();
            octree.Get<GameObject*>(ray, objectList);
        }

        
        struct Node;
        
        Octree octree;
        typedef std::vector<Node*> OctreeObjectsUpdateList;
		OctreeObjectsUpdateList octreeObjectsUpdateList;
        
        struct Node {
            
            Node(GameObject* object, Transform* transform, Mesh* mesh, OctreeSystem* system) :
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
                
                transform->World.HasBecomeDirty.Bind(this, &Node::TransformChanged);
                mesh->LocalBoundingBox.HasBecomeDirty.Bind(this, &Node::MeshBoundingBoxChanged);
                
                SetOctreeNodeDirty();
            }

            ~Node() {
                
                transform->World.HasBecomeDirty.Unbind(this, &Node::TransformChanged);
                mesh->LocalBoundingBox.HasBecomeDirty.Unbind(this, &Node::MeshBoundingBoxChanged);
                
                if (octreeNodeDirty) {
                    system->octreeObjectsUpdateList[indexInList] = 0;
                }
                
                if (octreeNode.node) {
                    system->octree.Remove(octreeNode);
                }
            }

            void TransformChanged() {
                boundingBoxDirty = true;
                SetOctreeNodeDirty();
            }

            void MeshBoundingBoxChanged() {
                boundingBoxDirty = true;
                SetOctreeNodeDirty();
            }

            bool InView(const BoundingFrustum& frustum) {
                if (boundingBoxDirty) {
                    boundingBoxDirty = false;
                    const BoundingBox& box = mesh->LocalBoundingBox();
                    box.CreateWorldAligned(transform->World, boundingBox);
                }
                return frustum.Intersect(boundingBox) != BoundingFrustum::OUTSIDE;
            }

            void SetOctreeNodeDirty() {
                if (octreeNodeDirty) return;
                octreeNodeDirty = true;
                indexInList = system->octreeObjectsUpdateList.size();
                system->octreeObjectsUpdateList.push_back(this);
            }

            void UpdateOctreeNode() {
                
                octreeNodeDirty = false;
                
                if (boundingBoxDirty) {
                    boundingBoxDirty = false;
                    const BoundingBox& box = mesh->LocalBoundingBox();
                    box.CreateWorldAligned(transform->World, octreeNode.box);
                }
                
                if (!octreeNode.node) {
                    system->octree.Insert(octreeNode);
                } else {
                    system->octree.Move(octreeNode);
                }
            }
                        
            Transform* transform;
            Mesh* mesh;
            OctreeSystem* system;
            Octree::Node octreeNode;
            BoundingBox boundingBox;
            bool octreeNodeDirty;
            bool boundingBoxDirty;
            size_t indexInList;
		};
    };
}