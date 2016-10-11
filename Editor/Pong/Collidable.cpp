#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "Event.hpp"
#include <iostream>

using namespace Pocket;

struct CollisionData {
    GameObject* a;
    GameObject* b; 
};

struct Collidable {
    float radius;

    Event<CollisionData> OnCollision;
};

struct CollisionSystem : public GameSystem<Transform, Collidable> {
    void Update(float dt) override {
        std::cout << "Objects : "<<Objects().size()<<std::endl;
        for(int i = 0; i<Objects().size(); ++i) {
            GameObject* a = Objects()[i];
            Transform* ta = a->GetComponent<Transform>();
            Collidable* ca = a->GetComponent<Collidable>();
            
            for(int j = i + 1; j<Objects().size(); ++j) {
                GameObject* b = Objects()[j];
                Transform* tb = b->GetComponent<Transform>();
                Collidable* cb = b->GetComponent<Collidable>();

                float totalRadii = ca->radius + cb->radius;

                Vector3 vector = ta->Position() - tb->Position();

                std::cout << vector << std::endl;

                if (vector.Length()<totalRadii) {
                    ca->OnCollision({ a, b });
                    cb->OnCollision({ b, a });
                }
            }
        }
    }
};