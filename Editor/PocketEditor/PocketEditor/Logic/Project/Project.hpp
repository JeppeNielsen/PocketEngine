//
//  Project.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"

using namespace Pocket;

struct Rotator {
    Vector3 speed;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(speed)
    TYPE_FIELDS_END
};
struct RotatorSystem : public GameSystem<Transform, Rotator> {
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->speed * dt;
        }
    }
};

class Project {
private:
    GameWorld world;
public:
    GameWorld& World();
    void CreateDefaultScene(GameWorld& editorWorld, GameObject* gameRoot);
};