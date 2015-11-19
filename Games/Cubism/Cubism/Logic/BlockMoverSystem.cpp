//
//  BlockRotaterSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockMoverSystem.hpp"
#include "Transform.hpp"
#include <iostream>
#include "Plane.hpp"
#include "Ray.hpp"
#include "InputManager.hpp"
#include "Camera.hpp"
#include <iostream>

void BlockMoverSystem::Initialize() {
    AddComponent<Touchable>();
    AddComponent<Transform>();
}

void BlockMoverSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Down += event_handler(this, &BlockMoverSystem::BlockDown, object);
    object->GetComponent<Touchable>()->Up += event_handler(this, &BlockMoverSystem::BlockUp, object);
}

void BlockMoverSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Down -= event_handler(this, &BlockMoverSystem::BlockDown, object);
    object->GetComponent<Touchable>()->Up -= event_handler(this, &BlockMoverSystem::BlockUp, object);
    MovingBlocks::iterator it = movingBlocks.find(object);
    if (it!=movingBlocks.end()) {
        movingBlocks.erase(it);
    }
}


void BlockMoverSystem::BlockDown(Pocket::TouchData touch, Pocket::GameObject *object) {
    if (touch.Index!=0) return;
    MovingBlock& movingBlock = movingBlocks[object];
    movingBlock.go = object->Parent(); //object;
    movingBlock.localPosition = movingBlock.go->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(touch.WorldPosition);
    movingBlock.touch = touch;
    movingBlock.touchOffset = touch.Position - touch.Camera->TransformPointToScreenSpace(touch.CameraObject->GetComponent<Transform>(), object->GetComponent<Transform>()->World.GetValue()->TransformPosition(0));
}


void BlockMoverSystem::BlockUp(Pocket::TouchData touch, Pocket::GameObject *object) {
    if (touch.Index!=0) return;
    MovingBlocks::iterator it = movingBlocks.find(object);
    if (it!=movingBlocks.end()) {
        movingBlocks.erase(it);
    }
}

void BlockMoverSystem::Update(float dt) {
    
    Transform* spaceTransform = space->GetComponent<Transform>();
    
    Plane plane({0,0,1}, -13);
    
    for (MovingBlocks::iterator it = movingBlocks.begin(); it!=movingBlocks.end(); ++it) {
        MovingBlock& movingBlock = it->second;
        
        Ray worldRay = movingBlock.touch.Camera->GetRay(movingBlock.touch.CameraObject->GetComponent<Transform>(), movingBlock.touch.Input->GetTouchPosition(movingBlock.touch.Index) - movingBlock.touchOffset);
        
        worldRay.Transform(*spaceTransform->WorldInverse.GetValue());
        
        float distance;
        if (plane.IntersectsRay(worldRay, &distance)) {
            Vector3 localPosition = worldRay.position + worldRay.direction * distance;
            Vector3 worldPosition = spaceTransform->World.GetValue()->TransformPosition(localPosition);
            
            
            
            Vector3 blockLocalPosition = movingBlock.go->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(worldPosition);
            
            Vector3 blockPosition = movingBlock.go->GetComponent<Transform>()->Local.GetValue()->TransformPosition(blockLocalPosition);
            /*
            std::cout<<"localPosition: "<<localPosition<<std::endl;
            std::cout<<"worldPosition: "<<worldPosition<<std::endl;
            std::cout<<"blockLocalPosition: "<<blockLocalPosition<<std::endl;
            std::cout<<"blockPosition: "<<blockPosition<<std::endl;
            std::cout<<"distance: "<<distance<<std::endl;
            std::cout<<"worldRay: "<<worldRay.position<<" direction:"<<worldRay.direction<<std::endl;
            */
            movingBlock.go->GetComponent<Transform>()->Position = blockPosition;
        }
        
    }
}



