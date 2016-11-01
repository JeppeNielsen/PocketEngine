//
//  UnitTests.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 11/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "LogicTests.hpp"
#include <cstdlib>
#include "GameWorld.hpp"
#include "GameSystem.hpp"

using namespace Pocket;


void LogicTests::RunTests() {
    
    AddTest("Create World", [] {
        GameWorld world;
        return world.Roots().size() == 0;
    });

    AddTest("Create Root", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        return root && world.Roots().size() == 1;
    });
    
    AddTest("Delete root", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        bool wasOne = world.Roots().size() == 1;
        root->Remove();
        world.Update(0);
        return wasOne && world.Roots().size() == 0;
    });
    
    AddTest("Create Child", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        return root && child && root->Children().size() == 1;
    });

    AddTest("Double Remove Child", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Children().size() == 1;
        child->Remove();
        child->Remove();
        world.Update(0);
        return wasOne && root->Children().size() == 0;
    });
    
    AddTest("Delete Child", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Children().size() == 1;
        child->Remove();
        world.Update(0);
        return wasOne && root->Children().size() == 0;
    });

    AddTest("Object Handle operator->, ", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        Handle<GameObject> handle = root;
        return handle.operator->() == root;
    });

    AddTest("Handle invalid on removed object ", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        Handle<GameObject> handle = root;
        bool wasValid = handle ? true : false;
        root->Remove();
        world.Update(0);
        return wasValid && !handle;
    });
    
    AddTest("GameSystem::Initialize", [] {
        static bool isSystemInitialized = false;
        struct Transform {};
        struct Velocity {};
        struct VelocitySystem : public GameSystem<Transform, Velocity> {
            void Initialize() override {
                isSystemInitialized = true;
            }
        };
    
        GameWorld world;
        world.AddSystemType<VelocitySystem>();
        world.CreateRoot();
        return isSystemInitialized;
    });
    
    AddTest("GameSystem::Destroy", [] {
        static bool isSystemDestroyed = false;
        struct Transform {};
        struct Velocity {};
        struct VelocitySystem : public GameSystem<Transform, Velocity> {
            void Destroy() override {
                isSystemDestroyed = true;
            }
        };
        
        {
            GameWorld world;
            world.AddSystemType<VelocitySystem>();
            world.CreateRoot();
        }
        return isSystemDestroyed;
    });
    
    AddTest("Two system instances", [] {
        static std::vector<void*> initializedSystems;
        struct Transform {};
        struct Velocity {};
        struct VelocitySystem : public GameSystem<Transform, Velocity> {
            void Initialize() override {
                initializedSystems.push_back(this);
            }
        };
    
        GameWorld world;
        world.AddSystemType<VelocitySystem>();
        world.CreateRoot();
        world.CreateRoot();
        return initializedSystems.size() == 2 &&
        initializedSystems[0]!=initializedSystems[1] &&
        initializedSystems[0] && initializedSystems[1];
    });
    
    AddTest("GameSystem::ObjectAdded", [] {
        static int objectsAdded = 0;
        struct Transform {};
        struct Velocity {};
        struct VelocitySystem : public GameSystem<Transform, Velocity> {
            void ObjectAdded(GameObject* object) override {
                objectsAdded++;
            }
        };
    
        GameWorld world;
        world.AddSystemType<VelocitySystem>();
        GameObject* root = world.CreateRoot();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        world.Update(0);
        
        return objectsAdded == 1;
    });

    AddTest("GameSystem::ObjectRemoved", [] {
        static int objectsAdded = 0;
        struct Transform {};
        struct Velocity {};
        struct VelocitySystem : public GameSystem<Transform, Velocity> {
            void ObjectAdded(GameObject* object) override {
                objectsAdded++;
            }
            void ObjectRemoved(GameObject* object) override {
                objectsAdded--;
            }
        };
    
        GameWorld world;
        world.AddSystemType<VelocitySystem>();
        GameObject* root = world.CreateRoot();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        world.Update(0);
        bool wasOne = objectsAdded == 1;
        root->Remove();
        world.Update(0);
        return wasOne && objectsAdded == 0;
    });

    AddTest("Disable Object", [] {
        static int objectsAdded = 0;
        struct Transform {};
        struct Velocity {};
        struct VelocitySystem : public GameSystem<Transform, Velocity> {
            void ObjectAdded(GameObject* object) override {
                objectsAdded++;
            }
            void ObjectRemoved(GameObject* object) override {
                objectsAdded--;
            }
        };
    
        GameWorld world;
        world.AddSystemType<VelocitySystem>();
        GameObject* root = world.CreateRoot();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        root->Enabled = false;
        world.Update(0);
        return objectsAdded == 0;
    });
    
    AddTest("CreateObject", []() {
        GameWorld world;
        world.CreateRoot();
        return world.ObjectCount()==1;
    });
    
    AddTest("Remove Object", []() {
        GameWorld world;
        GameObject* object = world.CreateRoot();
        bool wasOne = world.ObjectCount() == 1;
        object->Remove();
        world.Update(0);
        bool wasNone = world.ObjectCount() == 0;
        return wasOne && wasNone;
    });
    
    AddTest("GameWorld::Clear", []() {
        GameWorld world;
        world.CreateRoot();
        bool wasOne = world.ObjectCount() == 1;
        world.Clear();
        bool wasNone = world.ObjectCount() == 0;
        return wasOne && wasNone;
    });
    
    AddTest("Object::AddComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        return object && transform && transform->x == 4;
    });

    AddTest("double Object::AddComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        auto* transform2 = object->AddComponent<Transform>();
        return object && transform && transform->x == 4 && transform == transform2;
    });
    
    AddTest("Object::HasComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        return object && transform && object->HasComponent<Transform>();
    });
    
    AddTest("Object::GetComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        auto transform2 = object->GetComponent<Transform>();
        return object && transform && transform == transform2;
    });
    
    
    AddTest("Object::RemoveComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
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
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
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
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        auto object = world.CreateRoot();
        
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
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        GameObject* root = world.CreateRoot();
        
        auto source = root->CreateChild();
        auto sourceTransform = source->AddComponent<Transform>();
        sourceTransform->x = 123;
        
        
        auto copy = root->CreateChild();
        auto copyTransform = copy->AddComponent<Transform>(source);
        
        return sourceTransform && copyTransform && sourceTransform == copyTransform;
    });
    
    AddTest("Clone component", [](){
        struct Transform { Transform() : x(666) {} int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameWorld world;
        world.AddSystemType<TransformSystem>();
        GameObject* root = world.CreateRoot();
        auto source = root->CreateChild();
        auto sourceTransform = source->AddComponent<Transform>();
        sourceTransform->x = 123;
        
        auto copy = root->CreateChild();
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
        world.AddSystemType<RenderSystem>();
        GameObject* root = world.CreateRoot();
        RenderSystem* system = root->GetSystem<RenderSystem>();
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = root->CreateChild();
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
        world.AddSystemType<RenderSystem>();
        GameObject* root = world.CreateRoot();
        RenderSystem* system = root->GetSystem<RenderSystem>();
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = root->CreateChild();
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
            void Destroy() {
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
            world.AddSystemType<RenderSystem>();
            GameObject* root = world.CreateRoot();
            RenderSystem* system = root->GetSystem<RenderSystem>();
            system->Counter = &Counter;
            system->ConstuctorDestructorCounter = &ConstuctorDestructorCounter;
            GameObject* object = root->CreateChild();;
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
        world.AddSystemType<RenderSystem>();
        GameObject* root = world.CreateRoot();
        RenderSystem* system = root->GetSystem<RenderSystem>();
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = root->CreateChild();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOne = system->Objects().size() == 1;
        object->Enabled = false;
        world.Update(0);
        bool isNone = system->Objects().size() == 0;
        return wasNone && wasOne && isNone;
    });
    
    AddTest("GameObject::Enabled reset", []() {
        static int objectCounter = 0;
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Renderable> {
            void ObjectAdded(GameObject* o) override {
                objectCounter++;
            }
            void ObjectRemoved(GameObject* o) override {
                objectCounter--;
            }
        };
        
        GameWorld world;
        world.AddSystemType<RenderSystem>();
        GameObject* root = world.CreateRoot();
        auto object = root->CreateChild();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOneObject = objectCounter == 1;
        object->Enabled = false;
        world.Update(0);
        bool hadNoObjects = objectCounter == 0;
        object->Remove();
        world.Update(0);
        bool hadNoObjects2 = objectCounter == 0;
        auto objectSecondGen = root->CreateChild();
        bool sameObject = object == objectSecondGen;
        objectSecondGen->AddComponent<Renderable>();
        bool mustBeEnabled = objectSecondGen->Enabled;
        world.Update(0);
        bool wasOneObjectSecondTime = objectCounter == 1;
        
        return wasOneObject &&
        hadNoObjects &&
        hadNoObjects2 &&
        sameObject &&
        mustBeEnabled &&
        wasOneObjectSecondTime;
    });
    
    AddTest("Two roots, different system instances", []() {
        struct RenderSystem;
        static std::vector<RenderSystem*> renderSystems;
        struct Renderable { int imageNo; };
        struct RenderSystem : public GameSystem<Renderable> {
            void Initialize() override {
                renderSystems.push_back(this);
            }
        };
        
        GameWorld world;
        world.AddSystemType<RenderSystem>();
        world.CreateRoot();
        world.CreateRoot();
        
        return renderSystems.size() == 2 && renderSystems[0] && renderSystems[1] &&
        renderSystems[0]!=renderSystems[1];
    });

}
