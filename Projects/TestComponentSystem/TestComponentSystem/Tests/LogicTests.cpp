//
//  UnitTests.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 11/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "LogicTests.hpp"
#include "GameWorld.hpp"
#include <cstdlib>

using namespace Pocket;

void LogicTests::RunTests() {

    AddTest("CreateObject", []() {
        GameWorld world;
        world.CreateObject();
        return world.ObjectCount()==1;
    });
    
    AddTest("Remove Object", []() {
        GameWorld world;
        GameObject* object = world.CreateObject();
        bool wasOne = world.ObjectCount() == 1 && world.Root()->Children().size() == 1;
        object->Remove();
        world.Update(0);
        bool wasNone = world.ObjectCount() == 0 && world.Root()->Children().size() == 0;
        return wasOne && wasNone;
    });
    
    AddTest("GameWorld::Clear", []() {
        GameWorld world;
        world.CreateObject();
        bool wasOne = world.ObjectCount() == 1;
        world.Clear();
        bool wasNone = world.ObjectCount() == 0;
        return wasOne && wasNone;
    });
    
    AddTest("Default parent should be Root", []() {
        GameWorld world;
        GameObject* object = world.CreateObject();
        return object->Parent() == world.Root() && world.Root()->Children().size() == 1;
    });
    
    AddTest("Parent set to 0 should be child of root", []() {
        GameWorld world;
        GameObject* object = world.CreateObject();
        bool parentWasRoot = object->Parent() == world.Root();
        GameObject* parent = world.CreateObject();
        object->Parent() = parent;
        bool objectNotRootChild = std::find(world.Root()->Children().begin(), world.Root()->Children().end(), object)==world.Root()->Children().end();
        bool parentWasParent = object->Parent() == parent;
        object->Parent() = 0;
        bool objectIsRootChild = std::find(world.Root()->Children().begin(), world.Root()->Children().end(), object)!=world.Root()->Children().end();
        return parentWasRoot && parentWasParent && objectIsRootChild && objectNotRootChild;
    });
    
    AddTest("Object::AddComponent", []() {
        struct Transform { int x; };
        GameWorld world;
        auto object = world.CreateObject();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        return object && transform && transform->x == 4;
    });

    AddTest("double Object::AddComponent", []() {
        struct Transform { int x; };
        GameWorld world;
        auto object = world.CreateObject();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        auto* transform2 = object->AddComponent<Transform>();
        return object && transform && transform->x == 4 && transform == transform2;
    });
    
    AddTest("Object::HasComponent", []() {
        struct Transform { int x; };
        GameWorld world;
        auto object = world.CreateObject();
        auto transform = object->AddComponent<Transform>();
        return object && transform && object->HasComponent<Transform>();
    });
    
    AddTest("Object::GetComponent", []() {
        struct Transform { int x; };
        GameWorld world;
        auto object = world.CreateObject();
        auto transform = object->AddComponent<Transform>();
        auto transform2 = object->GetComponent<Transform>();
        return object && transform && transform == transform2;
    });
    
    
    AddTest("Object::RemoveComponent", []() {
        struct Transform { int x; };
        GameWorld world;
        auto object = world.CreateObject();
        auto transform = object->AddComponent<Transform>();
        transform->x = 123;
        bool firstTest = object && transform && transform->x == 123;
        object->RemoveComponent<Transform>();
        bool secondTest = object->HasComponent<Transform>() && object->GetComponent<Transform>()->x == 123;
        world.Update(0);
        bool thirdTest = !object->HasComponent<Transform>() && object->GetComponent<Transform>() == 0;
        return firstTest && secondTest && thirdTest;
    });
    
    AddTest("Component reset", []() {
        struct Transform { Transform() : x(666) {} int x; };
        GameWorld world;
        auto object = world.CreateObject();
        auto transform = object->AddComponent<Transform>();
        transform->x = 123;
        bool firstTest = object && transform && transform->x == 123;
        object->RemoveComponent<Transform>();
        world.Update(0);
        object->AddComponent<Transform>();
        bool thirdTest = object->GetComponent<Transform>()->x == 666;
        return firstTest && thirdTest;
    });
    
    AddTest("Multiple random Object::Add/RemoveComponent", []() {
        struct Transform { int x; };
        GameWorld world;
        auto object = world.CreateObject();
        
        for(int i=0; i<100; i++) {
            if (std::rand() % 2==0) {
                object->AddComponent<Transform>();
            } else {
                object->RemoveComponent<Transform>();
            }
        }
        object->RemoveComponent<Transform>();
        world.Update(0);
        return !object->HasComponent<Transform>();
    });
    
    AddTest("Reference component", [](){
        struct Transform { int x; };
        GameWorld world;
        auto source = world.CreateObject();
        auto sourceTransform = source->AddComponent<Transform>();
        sourceTransform->x = 123;
        
        
        auto copy = world.CreateObject();
        auto copyTransform = copy->AddComponent<Transform>(source);
        
        return sourceTransform && copyTransform && sourceTransform == copyTransform;
    });
    
    AddTest("Clone component", [](){
        struct Transform { Transform() : x(666) {} int x; };
        GameWorld world;
        auto source = world.CreateObject();
        auto sourceTransform = source->AddComponent<Transform>();
        sourceTransform->x = 123;
        
        auto copy = world.CreateObject();
        auto copyTransform = copy->CloneComponent<Transform>(source);
        
        return sourceTransform &&
        copyTransform &&
        sourceTransform != copyTransform &&
        sourceTransform->x == 123 &&
        copyTransform->x == 123;
    });
    
    AddTest("System::Object Count", [] () {
        struct Transform { int x; };
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Transform, Renderable> { };
        GameWorld world;
        RenderSystem* system = world.CreateSystem<RenderSystem>();
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOne = system->Objects().size() == 1;
        return wasNone && wasOne;
    });
    
    AddTest("GameWorld::Clear -> System::ObjectRemoved", [] () {
        struct Transform { int x; };
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Transform, Renderable> { };
        GameWorld world;
        RenderSystem* system = world.CreateSystem<RenderSystem>();
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOne = system->Objects().size() == 1;
        world.Clear();
        bool isNone = system->Objects().size() == 0;
        return wasNone && wasOne && isNone;
    });

    AddTest("GameWorld destructor cleanup ", [] () {
        struct Transform { int x; };
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Transform, Renderable> {
            int* Counter;
            int* ConstuctorDestructorCounter;
            ~RenderSystem() {
                (*ConstuctorDestructorCounter)++;
            }
            void ObjectAdded(GameObject* object) { (*Counter)++; }
            void ObjectRemoved(GameObject* object) { (*Counter)--; }
        };
        int Counter = 0;
        int ConstuctorDestructorCounter = 0;
        bool wasOne;
        {
            GameWorld world;
            RenderSystem* system = world.CreateSystem<RenderSystem>();
            system->Counter = &Counter;
            system->ConstuctorDestructorCounter = &ConstuctorDestructorCounter;
            GameObject* object = world.CreateObject();
            object->AddComponent<Transform>();
            object->AddComponent<Renderable>();
            world.Update(0);
            wasOne = Counter == 1;
        }
        return wasOne && Counter == 0 && ConstuctorDestructorCounter == 1;
    });

    AddTest("Object::Enabled", [] () {
        struct Transform { int x; };
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Transform, Renderable> { };
        GameWorld world;
        RenderSystem* system = world.CreateSystem<RenderSystem>();
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOne = system->Objects().size() == 1;
        object->Enabled() = false;
        world.Update(0);
        bool isNone = system->Objects().size() == 0;
        return wasNone && wasOne && isNone;
    });

    AddTest("Add System", []() {
        struct Transform { int x; };
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Transform, Renderable> {
        public:
            int numberOfObjects;
            void Initialize() { numberOfObjects = 0; }
            void ObjectAdded(GameObject* o) { numberOfObjects++; }
            void ObjectRemoved(GameObject* o) { numberOfObjects--; }
        };
        
        GameWorld world;
        auto object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        auto system = world.CreateSystem<RenderSystem>();
        bool wasOneObject = world.ObjectCount() == 1;
        world.Update(0);
        bool oneObjectInSystem = system->numberOfObjects == 1;
        world.Clear();
        bool wasZeroObjects = system->numberOfObjects == 0 && world.ObjectCount() == 0;
        auto object1 = world.CreateObject();
        object1->AddComponent<Transform>();
        object1->AddComponent<Renderable>();
        object1->Enabled() = false;
        auto object2 = world.CreateObject();
        object2->AddComponent<Transform>();
        object2->AddComponent<Renderable>();
        bool lastTest = world.ObjectCount() == 2;
        system = world.CreateSystem<RenderSystem>();
        world.Update(0);
        bool systemHasOnlyOneObject = system->numberOfObjects == 1;
        return wasOneObject && oneObjectInSystem && wasZeroObjects && lastTest && systemHasOnlyOneObject;
    });
    
    AddTest("Remove system", []() {
        static int ObjectCount = 0;
        struct Transform { int x; };
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Transform, Renderable> {
        public:
            void ObjectAdded(GameObject* o) { ObjectCount++; }
            void ObjectRemoved(GameObject* o) {ObjectCount--; }
        };
        
        GameWorld world;
        auto object = world.CreateObject();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        world.CreateSystem<RenderSystem>();
        bool wasOneObject = ObjectCount == 1;
        world.RemoveSystem<RenderSystem>();
        bool hasNoObjects = ObjectCount == 0;
        return wasOneObject && hasNoObjects;
    });
    
    
    
}
