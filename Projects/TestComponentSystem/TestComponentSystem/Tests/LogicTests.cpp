//
//  UnitTests.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 11/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "LogicTests.hpp"
#include <cstdlib>
#include "GameSystem.hpp"

using namespace Pocket;


void LogicTests::RunTests() {

    
    
    AddTest("Create World", [] {
        GameStorage storage;
        GameWorld world(storage);
        return world.Roots().size() == 0;
    });

    AddTest("Create Root", [] {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        return root && world.Roots().size() == 1;
    });
    
    AddTest("Delete root", [] {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        bool wasOne = world.Roots().size() == 1;
        root->Remove();
        world.Update(0);
        return wasOne && world.Roots().size() == 0;
    });
    
    AddTest("Create Child", [] {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        return root && child && root->Children().size() == 1;
    });

    AddTest("Double Remove Child", [] {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Children().size() == 1;
        child->Remove();
        child->Remove();
        world.Update(0);
        return wasOne && root->Children().size() == 0;
    });
    
    AddTest("Delete Child", [] {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Children().size() == 1;
        child->Remove();
        world.Update(0);
        return wasOne && root->Children().size() == 0;
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
    
        GameStorage storage;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
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
            GameStorage storage;
            storage.AddSystemType<VelocitySystem>();
            GameWorld world(storage);
            GameObject* root = world.CreateRoot();
            root->Remove();
            world.Update(0);
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
    
        GameStorage storage;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
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
        
        GameStorage storage;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
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
    
        GameStorage storage;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
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
    
        GameStorage storage;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        root->Enabled = false;
        world.Update(0);
        return objectsAdded == 0;
    });
    
    AddTest("CreateObject", []() {
        GameStorage storage;
        GameWorld world(storage);
        auto root = world.CreateRoot();
        return root->Children().size() == 1;
    });
    
    AddTest("Remove Object", []() {
//        GameStorage storage;
//        GameWorld world(storage);
//        GameObject* object = world.CreateRoot();
//        bool wasOne = world.ObjectCount() == 1;
//        object->Remove();
//        world.Update(0);
//        bool wasNone = world.ObjectCount() == 0;
        return false;// wasOne && wasNone;
    });
    
    AddTest("GameWorld::Clear", []() {
//        GameStorage storage;
//        GameWorld world(storage);
//        world.CreateRoot();
//        bool wasOne = world.ObjectCount() == 1;
//        world.Clear();
//        bool wasNone = world.ObjectCount() == 0;
//        return wasOne && wasNone;
          return false;
    });
    
    AddTest("Object::AddComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        return object && transform && transform->x == 4;
    });

    AddTest("double Object::AddComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        auto* transform2 = object->AddComponent<Transform>();
        return object && transform && transform->x == 4 && transform == transform2;
    });
    
    AddTest("Object::HasComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        return object && transform && object->HasComponent<Transform>();
    });
    
    AddTest("Object::GetComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        auto transform2 = object->GetComponent<Transform>();
        return object && transform && transform == transform2;
    });
    
    AddTest("Object::GetComponent is 0 on non owned component", []() {
        struct Transform { int x; };
        struct Velocity { int x; };
        struct VelocitySystem : public GameSystem<Transform, Velocity> {};
        GameStorage storage;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
        auto object = world.CreateRoot();
        auto transform = object->AddComponent<Transform>();
        auto velocity = object->GetComponent<Velocity>();
        return object && transform && !velocity;
    });

    
    AddTest("Object::RemoveComponent", []() {
        struct Transform { int x; };
        struct TransformSystem : public GameSystem<Transform> {};
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<RenderSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<RenderSystem>();
        GameWorld world(storage);
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
            GameStorage storage;
            storage.AddSystemType<RenderSystem>();
            GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<RenderSystem>();
        GameWorld world(storage);
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
        
        GameStorage storage;
        storage.AddSystemType<RenderSystem>();
        GameWorld world(storage);
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
        GameStorage storage;
        storage.AddSystemType<RenderSystem>();
        GameWorld world(storage);
        world.CreateRoot();
        world.CreateRoot();
        
        return renderSystems.size() == 2 && renderSystems[0] && renderSystems[1] &&
        renderSystems[0]!=renderSystems[1];
    });

    AddTest("Unique GameObject ids per root", []() {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        GameObject* object1 = root->CreateChild();
        GameObject* object2 = root->CreateChild();
        return object1->RootId()!=object2->RootId();
    });

    AddTest("Deleted GameObject new id", []() {
        GameStorage storage;
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        GameObject* object1 = root->CreateChild();
        int id1 = object1->RootId();
        object1->Remove();
        world.Update(0);
        GameObject* object2 = root->CreateChild();
        int id2 = object2->RootId();
        return id1!=id2 && object1 == object2;
    });
    
    AddTest("Remove object will remove component", [] () {
        struct TestComponent { int bla; };
        struct TestSystem : public GameSystem<TestComponent> { };
    
        GameStorage storage;
        storage.AddSystemType<TestSystem>();
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        
        GameObject* object1 = root->CreateChild();
        TestComponent* comp1 = object1->AddComponent<TestComponent>();
        object1->Remove();
        world.Update(0);
        GameObject* object2 = root->CreateChild();
        TestComponent* comp2 = object2->AddComponent<TestComponent>();
        
        return comp1 == comp2;
    });
    
    AddTest("Different root systems", [] () {
    
        struct Component1 {};
        struct Component2 {};
        
        struct System1 : public GameSystem<Component1> {};
        struct System2 : public GameSystem<Component2> {};
        GameStorage storage;
        storage.AddSystemType<System1>();
        storage.AddSystemType<System2>();
        
        GameWorld world(storage);
        GameObject* root1 = world.CreateRoot();
        
        GameObject* child1 = root1->CreateChild();
        Component1* comp1_1 = child1->AddComponent<Component1>();
        
        GameObject* root2 = world.CreateRoot();
        
        Component2* comp1_2 = child1->AddComponent<Component2>();
        
        GameObject* child2 = root2->CreateChild();
        Component1* comp2_1 = child2->AddComponent<Component1>();
        Component2* comp2_2 = child2->AddComponent<Component2>();
        
        return comp1_1 && comp1_2 && comp2_1 && comp2_2;
    });
    
    AddTest("System order", [] () {
        struct Component1 {};
        struct Component2 {};
        
        static std::vector<IGameSystem*> updatedSystems;
        
        struct System1 : public GameSystem<Component1> {
            void Update(float dt) { updatedSystems.push_back(this); }
        };
        struct System2 : public GameSystem<Component2> {
            void Update(float dt) { updatedSystems.push_back(this); }
        };
        
        GameStorage storage;
        storage.AddSystemType<System1>();
        storage.AddSystemType<System2>();
        
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        System1* s1 = root->GetSystem<System1>();
        System2* s2 = root->GetSystem<System2>();
        
        s1->Order = 2;
        s2->Order = 1;
        
        root->AddComponent<Component1>();
        root->AddComponent<Component2>();
        
        world.Update(0);
        
        return updatedSystems.size() == 2 &&
                updatedSystems[0] == s2 &&
                updatedSystems[1] == s1;
    });
    
    AddTest("Create child clone", [] () {
        struct Transform {
            int position;
        };
        
        struct Moveable {
            int speed;
        };
        
        struct MoverSystem : public GameSystem<Transform, Moveable> {};
    
        GameStorage storage;
        storage.AddSystemType<MoverSystem>();
        GameWorld world(storage);
        GameObject* sourceRoot = world.CreateRoot();

        GameObject* source = sourceRoot->CreateChild();
        source->AddComponent<Transform>()->position = 12345;
        source->AddComponent<Moveable>()->speed = 67890;
    
        GameObject* destRoot = world.CreateRoot();
        GameObject* clone = destRoot->CreateChildClone(source);
        
        return clone->GetComponent<Transform>() &&
               clone->GetComponent<Moveable>() &&
               clone->GetComponent<Transform>()->position == source->GetComponent<Transform>()->position &&
               clone->GetComponent<Moveable>()->speed == source->GetComponent<Moveable>()->speed &&
               clone->GetComponent<Transform>()!=source->GetComponent<Transform>() &&
               clone->GetComponent<Moveable>()!=source->GetComponent<Moveable>();
    });
    
    
    AddTest("Create copy", [] () {
        struct Transform {
            int position;
        };
        struct TransformSystem : public GameSystem<Transform> {};
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();

        GameObject* source = root->CreateChild();
        source->AddComponent<Transform>()->position = 12345;
        
        GameObject* copy = source->CreateCopy();
        
        return copy->GetComponent<Transform>() &&
                copy->GetComponent<Transform>()!=source->GetComponent<Transform>() &&
                copy->GetComponent<Transform>()->position == source->GetComponent<Transform>()->position &&
                source->Parent()==copy->Parent();
        });
    
    AddTest("Clone deep hierarchy", [] () {
        struct Transform {
            int position;
        };
        
        struct TransformSystem : public GameSystem<Transform> {};
    
        GameStorage storage;
        storage.AddSystemType<TransformSystem>();
        
        GameWorld world(storage);
        GameObject* sourceRoot = world.CreateRoot();
        
        GameObject* source = sourceRoot->CreateChild();
        source->AddComponent<Transform>()->position = 12345;
        
        GameObject* child = source->CreateChild();
        child->AddComponent<Transform>()->position = 67890;
        
        GameObject* destRoot = world.CreateRoot();
        GameObject* clone = destRoot->CreateChildClone(source);
        return true;
    });
    
    AddTest("Test add component no systems", [] () {
        struct Rotator {};
        struct Effect {};
        GameStorage storage;
        storage.AddComponentType<Rotator>();
        storage.AddComponentType<Effect>();
        
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        root->AddComponent<Rotator>();
        root->AddComponent<Effect>();
        return root->GetComponent<Rotator>() && root->GetComponent<Effect>();
    });
    
    AddTest("Default order of systems, two roots", [] () {
        
        static std::vector<IGameSystem*> systems;
        
        struct Rotator { };
        struct Effect { };
        struct RotatorSystem : public GameSystem<Rotator> {
            void Update(float dt) override {
                systems.push_back(this);
            }
        };
        struct EffectSystem : public GameSystem<Effect> {
            void Update(float dt) override {
                systems.push_back(this);
            }
        };
        
        GameStorage storage;
        storage.AddSystemType<RotatorSystem>();
        storage.AddSystemType<EffectSystem>();
        
        GameWorld world(storage);
        GameObject* root1 = world.CreateRoot();
        RotatorSystem* rotator1 = root1->GetSystem<RotatorSystem>();
        EffectSystem* effect1 = root1->GetSystem<EffectSystem>();
        
        GameObject* root2 = world.CreateRoot();
        RotatorSystem* rotator2 = root2->GetSystem<RotatorSystem>();
        EffectSystem* effect2 = root2->GetSystem<EffectSystem>();
        
        GameObject* obj1 = root1->CreateChild();
        obj1->AddComponent<Rotator>();
        obj1->AddComponent<Effect>();
        
        GameObject* obj2 = root2->CreateChild();
        obj2->AddComponent<Rotator>();
        obj2->AddComponent<Effect>();
        
        world.Update(1);
        
        return systems.size() == 4 &&
            systems[0] == root1->GetSystem<RotatorSystem>() &&
            systems[1] == root2->GetSystem<RotatorSystem>() &&
            systems[2] == root1->GetSystem<EffectSystem>() &&
            systems[3] == root2->GetSystem<EffectSystem>();
    });
    
    AddTest("ObjectAdded on CreateSystem", [] () {
        static int added = 0;
        
        struct Movable {};
        struct MoveSystem : public GameSystem<Movable> {
            void ObjectAdded(GameObject* go) {
                added++;
            }
        };
        
        GameStorage storage;
        storage.AddSystemType<MoveSystem>();
    
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        root->AddComponent<Movable>();
        world.Update(0);
        bool wasZero = added == 0;
        world.Update(0);
        return wasZero && added == 1;
    });
    
    AddTest("ObjectRemoved on RemoveSystem", [] () {
        static int removed = 0;
        
        struct Movable {};
        struct MoveSystem : public GameSystem<Movable> {
            void ObjectRemoved(GameObject* go) {
                removed++;
            }
        };
    
        GameStorage storage;
        storage.AddSystemType<MoveSystem>();
    
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();
        root->AddComponent<Movable>();
        world.Update(0);
        bool wasZero = removed == 0;

        world.Update(0);

        world.Update(0);
        return wasZero && removed == 1;
    });
    
    
    AddTest("ObjectRemoved on object->Remove()", [] () {
        static int removed = 0;
        
        struct Movable {};
        struct MoveSystem : public GameSystem<Movable> {
            void ObjectRemoved(GameObject* go) {
                removed++;
            }
        };
    
        GameStorage storage;
        storage.AddSystemType<MoveSystem>();
    
        GameWorld world(storage);
        GameObject* root = world.CreateRoot();

        GameObject* child = root->CreateChild();
        child->AddComponent<Movable>();
        world.Update(0);
        bool wasZero = removed == 0;
        child->Remove();
        world.Update(0);
        return wasZero && removed == 1;
    });
    
    AddTest("HasAncestor", [] () {
        
        GameStorage storage;
        GameWorld world(storage);
        
        GameObject* root = world.CreateRoot();
        GameObject* child1 = root->CreateChild();
        GameObject* child2 = root->CreateChild();
        
        GameObject* grandChild1 = child1->CreateChild();
        GameObject* grandChild2 = child2->CreateChild();
        
        GameObject* greatGrandChild1 = grandChild1->CreateChild();
        GameObject* greatGrandChild2 = grandChild2->CreateChild();
        
        return child1->HasAncestor(root) &&
               child2->HasAncestor(root) &&
               !child1->HasAncestor(child2) &&
               !root->HasAncestor(child1) &&
               grandChild1->HasAncestor(root) &&
               grandChild1->HasAncestor(child1) &&
               greatGrandChild1->HasAncestor(grandChild1) &&
               greatGrandChild2->HasAncestor(child2) &&
               greatGrandChild2->HasAncestor(root) &&
               !greatGrandChild1->HasAncestor(greatGrandChild2);
    });


    AddTest("Clone object with self reference components", [] () {
        
        struct Component1 {
            Component1() { val = 0; }
            int val;
        };
        
        GameStorage storage;
        storage.AddComponentType<Component1>();
        GameWorld world(storage);
        
        GameObject* prefab = world.CreateRoot();
        prefab->AddComponent<Component1>()->val = 123;
        GameObject* child = prefab->CreateChild();
        child->AddComponent<Component1>(prefab);
        
        GameObject* worldRoot = world.CreateRoot();
        GameObject* clone = worldRoot->CreateChildClone(prefab);
        
        Component1* comp = clone->GetComponent<Component1>();
        Component1* compChild = clone->Children()[0]->GetComponent<Component1>();
        
        return comp == compChild;
    });
    
    
    AddTest("Clone object with parent referencing child component", [] () {
        
        struct Component1 {
            Component1() { val = 0; }
            int val;
        };
        
        GameStorage storage;
        storage.AddComponentType<Component1>();
        GameWorld world(storage);
        
        GameObject* prefab = world.CreateRoot();
        GameObject* child = prefab->CreateChild();
        child->AddComponent<Component1>()->val = 433;
        prefab->AddComponent<Component1>(child);
        
        GameObject* worldRoot = world.CreateRoot();
        GameObject* clone = worldRoot->CreateChildClone(prefab);
        
        Component1* comp = clone->GetComponent<Component1>();
        Component1* compChild = clone->Children()[0]->GetComponent<Component1>();
        
        return comp == compChild;
    });

    


}
