
#include "Collidable.hpp"

void CollisionSystem::ObjectAdded(GameObject* object) {
    Collidable* col = object->GetComponent<Collidable>();
    std::cout << "Object added" << col->previousCollisions.size() << std::endl;
}

void CollisionSystem::ObjectRemoved(GameObject* object) {
    std::cout << "Object removed" << std::endl;
    object->GetComponent<Collidable>()->collisions.clear();
    object->GetComponent<Collidable>()->previousCollisions.clear();
}

void CollisionSystem::Update(float dt) {
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
                std::cout << "Enter"<<std::endl;
                collidable->Enter(c);
            }
        }

        for(auto c : collidable->previousCollisions) {
            auto current = std::find(collidable->collisions.begin(), collidable->collisions.end(), c);
            if (current == collidable->collisions.end()) {
                std::cout << "Exit"<<std::endl;
                collidable->Exit(c);
            }
        }
         collidable->previousCollisions = collidable->collisions;
         collidable->collisions.clear();
    }
}

