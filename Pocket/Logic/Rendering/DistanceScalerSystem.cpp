//
//  DistanceScalerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "DistanceScalerSystem.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;

void DistanceScalerSystem::Initialize() {
    renderSystem = root->CreateSystem<RenderSystem>();
}

void DistanceScalerSystem::Update(float dt) {
    auto cameras = renderSystem->GetCameras();
    if (cameras->Objects().empty()) return;

    for(auto cameraObject : cameras->Objects()) {
        Transform* cameraTransform = cameraObject->GetComponent<Transform>();
        const Matrix4x4& cameraWorldInverse = cameraTransform->WorldInverse();
        
        for(auto o : Objects()) {
            DistanceScaler* scaler = o->GetComponent<DistanceScaler>();
            Transform* transform = o->GetComponent<Transform>();
            Vector3 objectPosition = transform->World().TransformPosition(0);
            objectPosition = cameraWorldInverse.TransformPosition(objectPosition);
            float scale = scaler->Scale * objectPosition.z;
            if (scale<0) scale = -scale;
            transform->Scale = scale;
        }
    }
}
