
#include "RectCollider.hpp"

using namespace Pocket;

void RectCollisionSystem::ObjectAdded(GameObject* object) {
    RectCollider* col = object->GetComponent<RectCollider>();
    std::cout << "Object added" << col->previousCollisions.size() << std::endl;
}

void RectCollisionSystem::ObjectRemoved(GameObject* object) {
    std::cout << "Object removed" << std::endl;
    object->GetComponent<RectCollider>()->collisions.clear();
    object->GetComponent<RectCollider>()->previousCollisions.clear();
}

Box RectCollisionSystem::GetCollisionBox(GameObject* o) {

    Transform* t = o->GetComponent<Transform>();
    Sizeable* s = o->GetComponent<Sizeable>();

    Vector3 min = t->World().TransformPosition({0,0,0});
    Vector3 max = t->World().TransformPosition(s->Size());
    return Box(min.x, min.y, max.x, max.y);
}

void RectCollisionSystem::Update(float dt) {
    for(int i = 0; i<Objects().size(); ++i) {
        GameObject* a = Objects()[i];
        RectCollider* ca = a->GetComponent<RectCollider>(); 
        Box ba = GetCollisionBox(a);
        
        for(int j = i + 1; j<Objects().size(); ++j) {
            GameObject* b = Objects()[j];
            RectCollider* cb = b->GetComponent<RectCollider>();

            Box bb = GetCollisionBox(b);

            if (ba.Intersect(bb)) {
                ca->collisions.push_back(b);
                cb->collisions.push_back(a);
            }
        }
    }

    for(auto o : Objects()) {
        auto collider = o->GetComponent<RectCollider>();

        for(auto c : collider->collisions) {
            auto prev = std::find(collider->previousCollisions.begin(), collider->previousCollisions.end(), c);
            if (prev == collider->previousCollisions.end()) {
                std::cout << "Enter"<<std::endl;
                collider->Enter(c);
            }
        }

        for(auto c : collider->previousCollisions) {
            auto current = std::find(collider->collisions.begin(), collider->collisions.end(), c);
            if (current == collider->collisions.end()) {
                std::cout << "Exit"<<std::endl;
                collider->Exit(c);
            }
        }
        collider->previousCollisions = collider->collisions;
        collider->collisions.clear();
    }
}

