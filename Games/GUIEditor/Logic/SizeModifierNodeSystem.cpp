//
//  SizeModifierNodeSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SizeModifierNodeSystem.hpp"

void SizeModifierNodeSystem::ObjectAdded(GameObject *object) {
    draggingObjects.Add(object->GetComponent<Transform>()->Position, object);
}

void SizeModifierNodeSystem::ObjectRemoved(GameObject *object) {
   draggingObjects.Remove(object->GetComponent<Transform>()->Position, object);
}

void SizeModifierNodeSystem::Update(float dt) {

    const Vector2 gridSize = 10;
    
    for (GameObject* go : draggingObjects.Objects()) {
        Draggable* draggable = go->GetComponent<Draggable>();
        if (draggable->IsDragging) {
            SizeModifierNode* node = go->GetComponent<SizeModifierNode>();
            Vector3 position = go->GetComponent<Transform>()->World.GetValue()->TransformPosition(0);
            position.x = roundf(position.x / gridSize.x) * gridSize.x;
            position.y = roundf(position.y / gridSize.y) * gridSize.y;
            
            
            const Matrix4x4& worldInverse = *node->transformTarget->WorldInverse.GetValue();
            Vector3 localPosition = worldInverse.TransformPosition(position);
            
            Matrix4x4 localInverse = node->transformTarget->Local.GetValue()->Invert();
            
            Vector2 size = node->sizableTarget->Size;
            Vector3 localPivotPosition = localInverse.TransformPosition(node->transformTarget->Position);
            localPivotPosition.x = roundf(localPivotPosition.x / gridSize.x) * gridSize.x;
            localPivotPosition.y = roundf(localPivotPosition.y / gridSize.y) * gridSize.y;
            
            
            if (node->cornerIndex == 0 || node->cornerIndex == 1 || node->cornerIndex == 2) {
                size.x -= localPosition.x;
                localPivotPosition.x = localPosition.x + node->transformTarget->Anchor().x;
            } else if (node->cornerIndex == 4 || node->cornerIndex == 5 || node->cornerIndex == 6){
                size.x = localPosition.x;
            }
            
            if (node->cornerIndex == 0 || node->cornerIndex == 7 || node->cornerIndex == 6) {
                size.y -= localPosition.y;
                localPivotPosition.y = localPosition.y + node->transformTarget->Anchor().y;
            } else if (node->cornerIndex == 2 || node->cornerIndex == 3 || node->cornerIndex == 4){
                size.y = localPosition.y;
            }
            
            
            Vector3 targetPosition = node->transformTarget->Local.GetValue()->TransformPosition(localPivotPosition);
            
            size.x = roundf(size.x / gridSize.x) * gridSize.x;
            size.y = roundf(size.y / gridSize.y) * gridSize.y;
            
            node->transformTarget->Position = targetPosition;
            node->sizableTarget->Size = size;
        }
    }
    draggingObjects.Clear();
    
    for (GameObject* go : Objects()) {
        Draggable* draggable = go->GetComponent<Draggable>();
        if (draggable->IsDragging) continue;
        SizeModifierNode* node = go->GetComponent<SizeModifierNode>();
        Vector2 cornerPosition = node->GetCornerPosition();
        if (!node->sizableTarget) continue;
        go->GetComponent<Transform>()->Position = cornerPosition * node->sizableTarget->Size();
    }

}

