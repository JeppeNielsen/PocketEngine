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

static bool isSystemInitialized = false;
static bool isSystemDestroyed = false;
static std::vector<void*> initializedSystems;
static int objectsAdded = 0;
static int objectsRemoved = 0;

namespace LogicTestsComponents {

struct Transform { int x; int y; Transform() : x(666), y(0) {} };
struct Velocity { int vx; int vy; };
struct Renderable { int imageNo; };
struct NoSystemComponent { int bla; };

struct VelocitySystem : public GameSystem<Transform, Velocity> {
    void Initialize() override {
        isSystemInitialized = true;
        initializedSystems.push_back(this);
    }
    
    void Destroy() override {
        isSystemDestroyed = true;
    }
    
    void ObjectAdded(GameObject* object) override {
        objectsAdded++;
    }
    
    void ObjectRemoved(GameObject* object) override {
        objectsAdded--;
        objectsRemoved++;
    }
};

struct RenderSystem : public GameSystem<Transform, Renderable> { };

struct TestSubSystemsSystem : public GameSystem<Transform> {
    struct SubSystem : public GameSystem<Renderable> { };
public:
    static void CreateSubSystems(GameStorage& storage) {
        storage.AddSystemType<SubSystem>();
    }
};

}

using namespace LogicTestsComponents;

void LogicTests::RunTests() {

    storage.AddSystemType<VelocitySystem>();
    storage.AddSystemType<RenderSystem>();
    storage.AddComponentType<NoSystemComponent>();

    AddTest("Create World", [this] {
        GameWorld world(storage);
        return world.Scenes().size() == 0;
    });

    AddTest("Create Scene", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        return root && world.Scenes().size() == 1;
    });
    
    AddTest("Delete Scene", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        bool wasOne = world.Scenes().size() == 1;
        root->Remove();
        world.Update(0);
        return wasOne && world.Scenes().size() == 0;
    });
    
    AddTest("Create Child", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        GameObject* child = root->CreateChild();
        return root && child && root->Hierarchy().Children().size() == 1;
    });

    AddTest("Double Remove Child", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Hierarchy().Children().size() == 1;
        child->Remove();
        child->Remove();
        world.Update(0);
        return wasOne && root->Hierarchy().Children().size() == 0;
    });
    
    AddTest("Delete Child", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Hierarchy().Children().size() == 1;
        child->Remove();
        world.Update(0);
        return wasOne && root->Hierarchy().Children().size() == 0;
    });

    AddTest("GameSystem::Initialize", [this] {
        isSystemInitialized = false;
        GameWorld world(storage);
        world.CreateScene();
        return isSystemInitialized;
    });
    
    AddTest("GameSystem::Destroy", [this] {
        isSystemDestroyed = false;
        storage.AddSystemType<VelocitySystem>();
        GameWorld world(storage);
        GameObject* scene = world.CreateScene();
        scene->Remove();
        world.Update(0);
        return isSystemDestroyed;
    });
    
    AddTest("Two system instances", [this] {
        initializedSystems.clear();
        GameWorld world(storage);
        world.CreateScene();
        world.CreateScene();
        return initializedSystems.size() == 2 &&
        initializedSystems[0] != initializedSystems[1] &&
        initializedSystems[0] && initializedSystems[1];
    });
    
    AddTest("GameSystem::ObjectAdded", [this] {
        objectsAdded = 0;
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        world.Update(0);
        return objectsAdded == 1;
    });

    AddTest("GameSystem::ObjectRemoved", [this] {
        objectsAdded = 0;
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        world.Update(0);
        bool wasOne = objectsAdded == 1;
        root->Remove();
        world.Update(0);
        return wasOne && objectsAdded == 0;
    });

    AddTest("Disable Object", [this] {
        objectsAdded = 0;
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        root->AddComponent<Transform>();
        root->AddComponent<Velocity>();
        root->Hierarchy().Enabled = false;
        world.Update(0);
        return objectsAdded == 0;
    });
    
    AddTest("CreateObject", [this]() {
        GameWorld world(storage);
        auto scene = world.CreateScene();
        auto object = scene->CreateChild();
        return scene->Hierarchy().Children().size() == 1 && scene->Hierarchy().Children()[0] == object;
    });
    
    AddTest("RemoveObject", [this]() {
        GameWorld world(storage);
        auto scene = world.CreateScene();
        auto object = scene->CreateObject();
        bool wasOne = scene->Hierarchy().Children().size() == 1;
        object->Remove();
        world.Update(0);
        bool wasNone = scene->Hierarchy().Children().size() == 0;
        return wasOne && wasNone;
    });
    
    AddTest("GameWorld::Clear", [this]() {
        GameWorld world(storage);
        world.CreateScene();
        bool wasOne = world.Scenes().size() == 1;
        world.Clear();
        bool wasNone = world.Scenes().size() == 0;
        return wasOne && wasNone;
    });
    
    AddTest("Object::AddComponent", [this]() {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        return object && transform && transform->x == 4;
    });

    AddTest("double Object::AddComponent", [this]() {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        transform->x = 4;
        auto transform2 = object->AddComponent<Transform>();
        return object && transform && transform->x == 4 && transform == transform2;
    });
    
    AddTest("Object::HasComponent", [this]() {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        return object && transform && object->HasComponent<Transform>();
    });
    
    AddTest("Object::GetComponent", [this]() {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        auto transform2 = object->GetComponent<Transform>();
        return object && transform && transform == transform2;
    });
    
    AddTest("Object::GetComponent returns nullptr on non owned component", [this] {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        auto velocity = object->GetComponent<Velocity>();
        return object && transform && !velocity;
    });
    
    AddTest("Object::RemoveComponent", [this] {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        transform->x = 123;
        bool firstTest = object && transform && transform->x == 123;
        object->RemoveComponent<Transform>();
        bool secondTest = object->HasComponent<Transform>() && object->GetComponent<Transform>()->x == 123;
        world.Update(0);
        bool thirdTest = !object->HasComponent<Transform>() && object->GetComponent<Transform>() == 0;
        return firstTest && secondTest && thirdTest;
    });
    
    AddTest("Component reset", [this] {
        GameWorld world(storage);
        auto object = world.CreateScene();
        auto transform = object->AddComponent<Transform>();
        transform->x = 123;
        bool firstTest = object && transform && transform->x == 123;
        object->RemoveComponent<Transform>();
        world.Update(0);
        object->AddComponent<Transform>();
        bool thirdTest = object->GetComponent<Transform>()->x == 666;
        return firstTest && thirdTest;
    });
    
    AddTest("Multiple random Object::Add/RemoveComponent", [this] {
        GameWorld world(storage);
        auto object = world.CreateScene();
        
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
    
    AddTest("Reference component", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        
        auto source = root->CreateChild();
        auto sourceTransform = source->AddComponent<Transform>();
        sourceTransform->x = 123;
        
        
        auto copy = root->CreateChild();
        auto copyTransform = copy->AddComponent<Transform>(source);
        
        return sourceTransform && copyTransform && sourceTransform == copyTransform;
    });
    
    AddTest("Clone component", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();

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
    
    AddTest("System::Object Count", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        RenderSystem* system = root->GetSystem<RenderSystem>();

        bool wasNone = system->Objects().size() == 0;
        GameObject* object = root->CreateChild();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOne = system->Objects().size() == 1;
        return wasNone && wasOne;
    });
    
    AddTest("GameWorld::Clear -> System::ObjectRemoved", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
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

    AddTest("GameWorld destructor cleanup ", [] {
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
            GameObject* root = world.CreateScene();
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

    AddTest("Object::Enabled", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        RenderSystem* system = root->GetSystem<RenderSystem>();
        
        bool wasNone = system->Objects().size() == 0;
        GameObject* object = root->CreateChild();
        object->AddComponent<Transform>();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOne = system->Objects().size() == 1;
        object->Hierarchy().Enabled = false;
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
        GameObject* root = world.CreateScene();
        
        auto object = root->CreateChild();
        object->AddComponent<Renderable>();
        world.Update(0);
        bool wasOneObject = objectCounter == 1;
        object->Hierarchy().Enabled = false;
        world.Update(0);
        bool hadNoObjects = objectCounter == 0;
        object->Remove();
        world.Update(0);
        bool hadNoObjects2 = objectCounter == 0;
        auto objectSecondGen = root->CreateChild();
        bool sameObject = object == objectSecondGen;
        objectSecondGen->AddComponent<Renderable>();
        bool mustBeEnabled = objectSecondGen->Hierarchy().Enabled;
        world.Update(0);
        bool wasOneObjectSecondTime = objectCounter == 1;
        
        return wasOneObject &&
        hadNoObjects &&
        hadNoObjects2 &&
        sameObject &&
        mustBeEnabled &&
        wasOneObjectSecondTime;
    });
    
    AddTest("Two roots, different system instances", [] {
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
        world.CreateScene();
        world.CreateScene();
        
        return renderSystems.size() == 2 && renderSystems[0] && renderSystems[1] &&
        renderSystems[0]!=renderSystems[1];
    });

    AddTest("Unique GameObject ids per root", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        GameObject* object1 = root->CreateChild();
        GameObject* object2 = root->CreateChild();
        return object1->RootId()!=object2->RootId();
    });

    AddTest("Deleted GameObject new id", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        GameObject* object1 = root->CreateChild();
        int id1 = object1->RootId();
        object1->Remove();
        world.Update(0);
        GameObject* object2 = root->CreateChild();
        int id2 = object2->RootId();
        return id1!=id2 && object1 == object2;
    });
    
    AddTest("Remove object will remove component", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        
        GameObject* object1 = root->CreateChild();
        Transform* comp1 = object1->AddComponent<Transform>();
        object1->Remove();
        world.Update(0);
        GameObject* object2 = root->CreateChild();
        Transform* comp2 = object2->AddComponent<Transform>();
        return comp1 == comp2;
    });
    
    AddTest("Different scene systems", [] () {
    
        struct Component1 {};
        struct Component2 {};
        
        struct System1 : public GameSystem<Component1> {};
        struct System2 : public GameSystem<Component2> {};
        GameStorage storage;
        storage.AddSystemType<System1>();
        storage.AddSystemType<System2>();
        
        GameWorld world(storage);
        GameObject* root1 = world.CreateScene();
        
        GameObject* child1 = root1->CreateChild();
        Component1* comp1_1 = child1->AddComponent<Component1>();
        
        GameObject* root2 = world.CreateScene();
        
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
        GameObject* root = world.CreateScene();
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
        GameObject* sourceRoot = world.CreateScene();

        GameObject* source = sourceRoot->CreateChild();
        source->AddComponent<Transform>()->position = 12345;
        source->AddComponent<Moveable>()->speed = 67890;
    
        GameObject* destRoot = world.CreateScene();
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
        GameObject* root = world.CreateScene();

        GameObject* source = root->CreateChild();
        source->AddComponent<Transform>()->position = 12345;
        
        GameObject* copy = source->CreateCopy();
        
        return copy->GetComponent<Transform>() &&
                copy->GetComponent<Transform>()!=source->GetComponent<Transform>() &&
                copy->GetComponent<Transform>()->position == source->GetComponent<Transform>()->position &&
                source->Hierarchy().Parent()==copy->Hierarchy().Parent();
        });
    
    AddTest("Clone deep hierarchy", [this] {
        GameWorld world(storage);
        GameObject* sourceRoot = world.CreateScene();
        
        GameObject* source = sourceRoot->CreateChild();
        source->AddComponent<Transform>()->x = 12345;
        
        GameObject* child = source->CreateChild();
        child->AddComponent<Transform>()->x = 67890;
        
        GameObject* destRoot = world.CreateScene();
        GameObject* clone = destRoot->CreateChildClone(source);
        return source->GetComponent<Transform>()->x ==
               clone->GetComponent<Transform>()->x &&
               clone->Hierarchy().Children()[0]->GetComponent<Transform>()->x == child->GetComponent<Transform>()->x;
    });
    
    AddTest("Test add component no systems", [this] {
        GameWorld world(storage);
        GameObject* root = world.CreateScene();
        root->AddComponent<Transform>();
        root->AddComponent<Renderable>();
        return root->GetComponent<Transform>() && root->GetComponent<Renderable>();
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
        GameObject* root1 = world.CreateScene();
        RotatorSystem* rotator1 = root1->GetSystem<RotatorSystem>();
        EffectSystem* effect1 = root1->GetSystem<EffectSystem>();
        
        GameObject* root2 = world.CreateScene();
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
    
    AddTest("ObjectRemoved on object->Remove()", [this] {
        objectsRemoved = 0;
        GameWorld world(storage);
        GameObject* root = world.CreateScene();

        GameObject* child = root->CreateChild();
        child->AddComponent<Transform>();
        child->AddComponent<Velocity>();
        world.Update(0);
        bool wasZero = objectsRemoved == 0;
        child->Remove();
        world.Update(0);
        return wasZero && objectsRemoved == 1;
    });
    
    AddTest("HasAncestor", [this] {
        GameWorld world(storage);
        
        GameObject* root = world.CreateScene();
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
        
        GameObject* prefab = world.CreateScene();
        prefab->AddComponent<Component1>()->val = 123;
        GameObject* child = prefab->CreateChild();
        child->AddComponent<Component1>(prefab);
        
        GameObject* worldRoot = world.CreateScene();
        GameObject* clone = worldRoot->CreateChildClone(prefab);
        
        Component1* comp = clone->GetComponent<Component1>();
        Component1* compChild = clone->Hierarchy().Children()[0]->GetComponent<Component1>();
        
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
        
        GameObject* prefab = world.CreateScene();
        GameObject* child = prefab->CreateChild();
        child->AddComponent<Component1>()->val = 433;
        prefab->AddComponent<Component1>(child);
        
        GameObject* worldRoot = world.CreateScene();
        GameObject* clone = worldRoot->CreateChildClone(prefab);
        
        Component1* comp = clone->GetComponent<Component1>();
        Component1* compChild = clone->Hierarchy().Children()[0]->GetComponent<Component1>();
        
        return comp == compChild;
    });
    
    
    AddTest("Child world disabled, when parent disabled", [this] () {
        
        GameWorld world(storage);
        
        auto scene = world.CreateScene();
        scene->Hierarchy().Enabled = false;
        auto child = scene->CreateChild();

        bool isEnabled = child->Hierarchy().Enabled;
        bool isWorldEnabled = child->Hierarchy().WorldEnabled();
        
        return isEnabled && !isWorldEnabled;
    });
 
    AddTest("Child world disabled and enabled by parent", [this] () {
        
        GameWorld world(storage);
        
        auto scene = world.CreateScene();
        scene->Hierarchy().Enabled = false;
        auto child = scene->CreateChild();
        
        bool wasWorldEnabled = child->Hierarchy().WorldEnabled();
        scene->Hierarchy().Enabled = true;
        bool isWorldEnabled = child->Hierarchy().WorldEnabled();
        
        return !wasWorldEnabled && isWorldEnabled;
    });
    
    
    AddTest("Test subsystem", [this] () {
        
        GameStorage storage;
        storage.AddSystemType<TestSubSystemsSystem>();
        GameWorld world(storage);
        
        auto scene = world.CreateScene();
        auto system1 = scene->GetSystem<TestSubSystemsSystem>();
        auto system2 = scene->GetSystem<TestSubSystemsSystem::SubSystem>();
        
        return system1 && system2;
    });
    
    AddTest("GameWorld dtor will call dtor on system", [this] () {
        
        struct DestroyedComponent {};
        
        static bool wasDestroyed = false;
        
        struct DestroyedSystem : GameSystem<DestroyedComponent> {
            ~DestroyedSystem () {
                wasDestroyed = true;
            }
        };
        
        GameStorage storage;
        storage.AddSystemType<DestroyedSystem>();
        
        {
            GameWorld world(storage);
            auto scene = world.CreateScene();
        }
        return wasDestroyed;
    });
    
    AddTest("Event listeners reset on component reset", [this] () {
        
        struct Bomb {
            void InvokeExplode() {
                Exploded(1);
            }
            Event<int> Exploded;
        };
        
        struct BombSystem : GameSystem<Bomb> {
            
        };
        
        GameStorage storage;
        storage.AddSystemType<BombSystem>();
        GameWorld world(storage);
        auto scene = world.CreateScene();
        
        auto bombObject = scene->CreateObject();
        auto bomb = bombObject->AddComponent<Bomb>();
        bomb->Exploded.Bind([] (int n) {
            
        });
        bool wasNotEmpty = !bomb->Exploded.Empty();
        world.Update(0);
        bombObject->Remove();
        world.Update(0);
        
        auto bombObject2 = scene->CreateObject();
        auto bomb2 = bombObject2->AddComponent<Bomb>();
        bool isEmpty = bomb2->Exploded.Empty();
        world.Update(0);
        
        return wasNotEmpty && isEmpty && bomb == bomb2;
    });
    
    AddTest("Event listeners reset on properties on component reset", [this] () {
        
        struct Bomb {
            Property<int> Size;
        };
        
        struct BombSystem : GameSystem<Bomb> {
            
        };
        
        GameStorage storage;
        storage.AddSystemType<BombSystem>();
        GameWorld world(storage);
        auto scene = world.CreateScene();
        
        auto bombObject = scene->CreateObject();
        auto bomb = bombObject->AddComponent<Bomb>();
        bomb->Size.Changed.Bind([] {});
        bool wasNotEmpty = !bomb->Size.Changed.Empty();
        world.Update(0);
        bombObject->Remove();
        world.Update(0);
        
        auto bombObject2 = scene->CreateObject();
        auto bomb2 = bombObject2->AddComponent<Bomb>();
        bool isEmpty = bomb2->Size.Changed.Empty();
        world.Update(0);
        
        return wasNotEmpty && isEmpty && bomb == bomb2;
    });
    
    AddTest("Hierarchy Enabled event reset on new object", [this] () {
        
        GameStorage storage;
        GameWorld world(storage);
        auto scene = world.CreateScene();
        
        bool changed = false;
        auto object1 = scene->CreateObject();
        object1->Hierarchy().Enabled = false;
        object1->Hierarchy().Enabled.Changed.Bind([&] {
            changed = true;
        });
        world.Update(0);
        bool wasChanged = !changed;
        object1->Remove();
        world.Update(0);
        auto object2 = scene->CreateObject();

        bool isChanged = !changed;
        
        return wasChanged && isChanged;
    });
    
 
}
