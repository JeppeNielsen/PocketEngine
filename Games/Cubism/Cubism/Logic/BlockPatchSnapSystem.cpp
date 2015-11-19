//
//  BlockPatchSnapSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockPatchSnapSystem.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"

void BlockPatchSnapSystem::Initialize() {
    AddComponent<Touchable>();
    AddComponent<Transform>();
}

void BlockPatchSnapSystem::AddedToWorld(Pocket::GameWorld &world) {
    octree = world.CreateSystem<OctreeSystem>();
    octree->AddComponent<Patch>();
}

void BlockPatchSnapSystem::ObjectAdded(GameObject* object) {
    object->GetComponent<Touchable>()->Down += event_handler(this, &BlockPatchSnapSystem::Down);
    object->GetComponent<Touchable>()->Up += event_handler(this, &BlockPatchSnapSystem::Up);
}

void BlockPatchSnapSystem::ObjectRemoved(GameObject* object) {
    object->GetComponent<Touchable>()->Down -= event_handler(this, &BlockPatchSnapSystem::Down);
    object->GetComponent<Touchable>()->Up -= event_handler(this, &BlockPatchSnapSystem::Up);
    auto it = touchingBlocks.find(object->GetComponent<Touchable>());
    if (it!=touchingBlocks.end()) {
        touchingBlocks.erase(it);
    }
}

void BlockPatchSnapSystem::Down(Pocket::TouchData d) {
    if (d.Index!=0) return;
    TouchingBlock& touchingBlock = touchingBlocks[d.Touchable];
    touchingBlock.touch = d;
    touchingBlock.go = d.object->Parent();
    touchingBlock.screenSpaceCenterOffset = d.Position - d.Camera->TransformPointToScreenSpace(d.CameraObject->GetComponent<Transform>(), touchingBlock.go->GetComponent<Transform>()->World.GetValue()->TransformPosition(0));
    
    Block* block = touchingBlock.go->GetComponent<Block>();

    block->Moving = true;
}

void BlockPatchSnapSystem::Up(Pocket::TouchData d) {
    if (d.Index!=0) return;
    auto it = touchingBlocks.find(d.Touchable);
    if (it!=touchingBlocks.end()) {
        Block* block = it->second.go->GetComponent<Block>();
        block->Moving = false;
        touchingBlocks.erase(it);
    }
}

void BlockPatchSnapSystem::Update(float dt) {
    
    for (TouchingBlocks::iterator it = touchingBlocks.begin(); it!=touchingBlocks.end(); ++it) {
        UpdateTouchingBlock(it->second);
    }
    
    /*
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* go = *it;
        if (go->GetComponent<Block>()->Patch) {
            go->GetComponent<Transform>()->Position = 0;
        }
    }
     */
    
}

bool BlockPatchSnapSystem::Sort(const BlockPatchSnapSystem::LinePick &a, const BlockPatchSnapSystem::LinePick &b) {
    return a.distance<b.distance;
}

void BlockPatchSnapSystem::UpdateTouchingBlock(BlockPatchSnapSystem::TouchingBlock &touchingBlock) {

    Block* block = touchingBlock.go->GetComponent<Block>();
    Patch* oldPatch = block->Patch() ? block->Patch()->GetComponent<Patch>() : 0;
    
    //Transform* transform = touchingBlock.go->GetComponent<Transform>();
    //Matrix4x4* world = transform->World.GetValue();
    Vector2 screenPosition =touchingBlock.touch.Input->GetTouchPosition(touchingBlock.touch.Index) - touchingBlock.screenSpaceCenterOffset;
    
    Ray ray = camera->GetRay(cameraObject->GetComponent<Transform>(), screenPosition);
    Vector3 forward = -ray.direction.Normalized();
    
    
    //ray.position = world->TransformPosition(0);
    //ray.direction = world->TransformVector({0,0,-1});

    ObjectCollection objectsSelected;
    
    octree->GetObjectsAtRay(ray, objectsSelected);
    
    std::vector<LinePick> linePicks;
    
    for (int i=0; i<objectsSelected.size(); i++) {
        Patch* patch = objectsSelected[i]->GetComponent<Patch>();
        if (patch->BlockAttached && patch->BlockAttached.Get()!=touchingBlock.go) continue;
        
        Transform* transform = objectsSelected[i]->GetComponent<Transform>();
        
        if (patch!=oldPatch) {
            Vector3 patchNormal = transform->World.GetValue()->TransformVector({0,0,1});
            patchNormal.Normalize();
        
        
            float angle = acosf(forward.Dot(patchNormal));
        
            float degrees = ToDegrees(angle);
            if (degrees>65) continue;
        }
        
        Mesh* mesh = objectsSelected[i]->GetComponent<Mesh>();
        
        Ray localRay = ray;
        localRay.Transform(*transform->WorldInverse.GetValue());
        
        float pickDistance;
        float bU, bV;
        size_t tri;
        Vector3 normal;
        if (mesh->IntersectsRay(localRay, &pickDistance, &bU, &bV, &tri, &normal)) {
            
            Vector3 localPickPosition = localRay.position + localRay.direction * pickDistance;
            Vector3 worldPickPosition = transform->World.GetValue()->TransformPosition(localPickPosition);
            
            LinePick lp;
            lp.go = objectsSelected[i];
            lp.distance = camera->TransformPointToScreenSpace(cameraObject->GetComponent<Transform>(), worldPickPosition).z;
            linePicks.push_back(lp);
        }
    }
    
    std::sort(linePicks.begin(), linePicks.end(), BlockPatchSnapSystem::Sort);
    
    Patch* foundPatch = 0;
    GameObject* foundPatchObject = 0;
    
    if (linePicks.size()>0) {
        foundPatchObject =linePicks[0].go;
        foundPatch = foundPatchObject->GetComponent<Patch>();
    }
    
    
    if (!foundPatch) {
        block->Patch = 0;
        if (oldPatch) {
            oldPatch->BlockAttached = 0;
        }
    } else {
        if (oldPatch!=foundPatch) {
            if (oldPatch) {
                oldPatch->BlockAttached = 0;
            }
            block->Patch = foundPatchObject;
            foundPatch->BlockAttached = touchingBlock.go;
        }
    }
}
