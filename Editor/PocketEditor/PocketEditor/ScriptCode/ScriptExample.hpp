

#include <iostream>
#include <vector>
#include "GameSystem.hpp"
#include "Property.hpp"
#include "Transform.hpp"

using namespace Pocket;

struct Velocity {
    Velocity() : velocity(0) {}
    Vector3 velocity;
    std::string name;
    std::vector<std::string> children;
    Pocket::Property<int> Size;
    std::vector<double> coords;
};

struct MovementSystem : public GameSystem<Transform, Velocity> {

    void ObjectAdded(GameObject* object) {
    
        std::vector<int> bla;
        bla.push_back(2);
        bla.push_back(3);
        
        int sum = 0;
        for(auto i : bla) sum+=i;
        
        object->GetComponent<Velocity>()->children.push_back("First");
        object->GetComponent<Velocity>()->children.push_back("Second");
        object->GetComponent<Velocity>()->children.push_back("Third");
        object->GetComponent<Velocity>()->Size = 43;
        
        auto& c = object->GetComponent<Velocity>()->coords;
        c.push_back(43.22);
        c.push_back(43.22*2);
        c.push_back(43.22*5);
        
        
        std::cout << "MovementSystem::ObjectAdded "<<sum<<std::endl;
    }
 
    void ObjectRemoved(GameObject* object) {
        std::cout << "MovementSystem::ObjectRemoved"<<std::endl;
    }
 
    void Update(float dt) {
        for(auto o : Objects()) {
            Transform* t = o->GetComponent<Transform>();
            Velocity* v = o->GetComponent<Velocity>();
            t->Position += v->velocity * dt;
            //o->GetComponent<Position>()->x += o->GetComponent<Velocity>()->vx;
            //o->GetComponent<Position>()->y += o->GetComponent<Velocity>()->vy;
            //std::cout << " x:" << o->GetComponent<Position>()->x<<
            //    " y:" << o->GetComponent<Position>()->y<<std::endl;
        }
    }
};