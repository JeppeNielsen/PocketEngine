#include "TransformFollower.hpp"
#include <iostream>

void TransformFollowerSystem::Initialize() {
    Order = 100;
}   

void TransformFollowerSystem::ObjectAdded(GameObject* object) {

}

void TransformFollowerSystem::ObjectRemoved(GameObject* object) {

}

void TransformFollowerSystem::Update(float dt) {
    for(auto o : Objects()) {
        auto transformFollower = o->GetComponent<TransformFollower>();
        
        GameObject* targetObject = transformFollower->target();
        if (!targetObject) continue;
        
        auto transform = o->GetComponent<Transform>();
        auto targetTransform = targetObject->GetComponent<Transform>();
        if (!targetTransform) continue;

        transform->Position = Vector3::Lerp(
                transform->Position, 
                targetTransform->World().TransformPosition(transformFollower->position), 
                transformFollower->smoothness);

        Vector3 lookAt = targetTransform->World().TransformPosition(transformFollower->lookAt);
        transform->Rotation = Quaternion::LookAt(transform->Position, lookAt, {0,1,0});
    }
}
