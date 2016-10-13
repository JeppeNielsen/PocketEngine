#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "Event.hpp"
#include <iostream>

using namespace Pocket;

struct Collidable {
    float radius;

    std::vector<GameObject*> collisions;
    std::vector<GameObject*> previousCollisions;
    
    Event<GameObject*> Enter;
    Event<GameObject*> Exit;
};

struct CollisionSystem : public GameSystem<Transform, Collidable> {
    void Update(float dt) override {
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
                    
                    ca->collisions.push_back(b);
                    cb->collisions.push_back(a);
                }
            }
        }

        for(auto o : Objects()) {
            auto collidable = o->GetComponent<Collidable>();

            for(auto c : collidable->collisions) {
                auto prev = std::find(collidable->previousCollisions.begin(), collidable->previousCollisions.end(), c);
                if (prev == collidable->previousCollisions.end()) {
                    collidable->Enter(c);
                }
            }

            for(auto c : collidable->previousCollisions) {
                auto current = std::find(collidable->collisions.begin(), collidable->collisions.end(), c);
                if (current == collidable->collisions.end()) {
                    collidable->Exit(c);
                }
            }
             collidable->previousCollisions = collidable->collisions;
             collidable->collisions.clear();
        }
    }
};