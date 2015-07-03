//
//  MeshOctreeSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/20/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameWorld.hpp"
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Octree.hpp"

namespace Pocket {
    class MeshOctreeSystem : public GameSystem {
    public:
        void Initialize();
        void GetObjectsInFrustum(const BoundingFrustum& frustum, ObjectCollection& objectList);
        void GetObjectsAtRay(const Ray& ray, ObjectCollection& objectList);
        void SetWorldBounds(const BoundingBox& bounds);
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        
        void UpdateAllNodes();
        
        struct Node;
        
        Octree octree;
        typedef std::vector<Node*> OctreeObjectsUpdateList;
		OctreeObjectsUpdateList octreeObjectsUpdateList;
        
        struct Node {
            Node(GameObject* object, Transform* transform, MeshComponent* mesh, MeshOctreeSystem* system);
            ~Node();
            
            void TransformChanged(DirtyProperty<Transform*, Matrix4x4>* transform);
			void MeshBoundingBoxChanged(DirtyProperty<MeshComponent*, BoundingBox>* mesh);
			bool InView(const BoundingFrustum& frustum);
			void SetOctreeNodeDirty();
			void UpdateOctreeNode();
            
            Transform* transform;
            MeshComponent* mesh;
            MeshOctreeSystem* system;
            Octree::Node octreeNode;
            BoundingBox boundingBox;
            bool octreeNodeDirty;
            bool boundingBoxDirty;
            size_t indexInList;
		};
    };
}