//
//  Tests.cpp
//  Pocket
//
//  Created by Jeppe Nielsen on 9/30/14.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ComponentSystemTests.hpp"
#include "UnitTest.hpp"
#include "GameWorld.hpp"
#include "Property.hpp"

using namespace Pocket;

void ComponentSystemTests::Run() {

    UnitTest("CreateObject", []() {
        GameWorld world;
        auto object = world.CreateObject();
        return object != 0;
    });
    
    UnitTest("Create 2 objects", []()->bool {
        GameWorld world;
        auto obj1 = world.CreateObject();
        auto obj2 = world.CreateObject();
        return obj1 && obj2 && obj1!=obj2 && world.ObjectCount()==2;
    });
    
    UnitTest("Delete all objects", []()->bool {
        GameWorld world;
        for (int i=0; i<100; i++) {
            world.CreateObject();
        }
        bool wasHundred = world.ObjectCount()==100;
        world.Clear();
        return world.ObjectCount() == 0 && wasHundred;
    });
    
    UnitTest("Create Component", []()->bool {
        struct Position {
            Position() { position = 5; }
            int position;
        };
        struct PositionSystem : public GameSystem<Position> {};
        
        GameWorld world;
        world.CreateSystem<PositionSystem>();
        auto object = world.CreateObject();
        auto position = object->AddComponent<Position>();
        return position!=0;
    });
    
    UnitTest("Reset Component", []()->bool {
        struct Position {
            Position() { position = 5; }
            int position;
        };
        struct PositionSystem : GameSystem<Position> {};
        GameWorld world;
        world.CreateSystem<PositionSystem>();
        auto object = world.CreateObject();
        auto position = object->AddComponent<Position>();
        return position->position==5;
    });
    
    UnitTest("GetComponent", []()->bool {
        struct Position {
            Position() { position = 5; }
            int position;
        };
        struct PositionSystem : GameSystem<Position> {};
        GameWorld world;
        world.CreateSystem<PositionSystem>();
        auto object = world.CreateObject();
        object->AddComponent<Position>();
        return object->GetComponent<Position>()!=0;
    });

    
    UnitTest("RemoveComponent", []()->bool {
        struct Position {
            Position() { position = 5; }
            int position;
        };
        struct PositionSystem : GameSystem<Position> {};
        GameWorld world;
        world.CreateSystem<PositionSystem>();
        auto object = world.CreateObject();
        bool componentDidExist = object->AddComponent<Position>()!=0;
        world.Update(0);
        object->RemoveComponent<Position>();
        world.Update(0);
        return componentDidExist && object->GetComponent<Position>()==0;
    });
    
    UnitTest("EnableComponent", []()->bool {
        struct Position {
            Position() { position = 5; }
            int position;
        };
        struct PositionSystem : GameSystem<Position> {};
        GameWorld world;
        world.CreateSystem<PositionSystem>();
        auto object = world.CreateObject();
        object->AddComponent<Position>();
        world.Update(0);
        bool wasEnabled = false;//object->IsComponentEnabled<Position>();
        //object->EnableComponent<Position>(false);
        world.Update(0);
        bool isEnabled = false;//object->IsComponentEnabled<Position>();
        world.Update(0);
        return wasEnabled && !isEnabled;
    });
    
    UnitTest("Add System", []()->bool {
        struct Position {
            Position() { counter = 0; position = 5; }
            int counter;
            int position;
        };
        
        struct Velocity {
        public:
            int velocity;
            Velocity() { velocity = 0; }
        };
        
        struct VelocitySystem : public GameSystem<Position, Velocity> {
            void ObjectAdded(GameObject* object) {
                object->GetComponent<Position>()->counter++;
            }
            void ObjectRemoved(GameObject* object) {
                object->GetComponent<Position>()->counter--;
            }
        };
        
        GameWorld world;
        VelocitySystem* system = world.CreateSystem<VelocitySystem>();
        return system!=0;
    });
    
    UnitTest("Object added to system", []()->bool {
        struct Position {
            Position() { counter = 0; position = 5; }
            int counter;
            int position;
        };
        
        struct Velocity {
        public:
            int velocity;
            Velocity() { velocity = 0; }
        };
        struct VelocitySystem : public GameSystem<Position, Velocity> {
            void ObjectAdded(GameObject* object) {
                object->GetComponent<Position>()->counter++;
            }
            void ObjectRemoved(GameObject* object) {
                object->GetComponent<Position>()->counter--;
            }
        };
             
        GameWorld world;
        world.CreateSystem<VelocitySystem>();
             
        auto o1 = world.CreateObject();
        o1->AddComponent<Position>();
        o1->AddComponent<Velocity>();
        auto o2 = world.CreateObject();
        o2->AddComponent<Position>();
        world.Update(0);
        return
             o1->GetComponent<Position>()->counter == 1 &&
             o2->GetComponent<Position>()->counter == 0;
    });
    
    UnitTest("Object added and removed from system", []()->bool {
        struct Position {
            Position() { counter = 0; position = 5; }
            int counter;
            int position;
        };
        
        struct Velocity {
        public:
            int velocity;
            Velocity() { velocity = 0; }
        };
        
        struct VelocitySystem : public GameSystem<Position, Velocity> {
            void ObjectAdded(GameObject* object) {
                object->GetComponent<Position>()->counter++;
                objectCounter++;
            }
            void ObjectRemoved(GameObject* object) {
                object->GetComponent<Position>()->counter--;
                objectCounter--;
            }
        public:
            int objectCounter = 0;
        };
        
             
         GameWorld world;
         auto velocitySystem = world.CreateSystem<VelocitySystem>();
         
         GameObject* objects[10];
         for(int i=0; i<10; i++) {
             objects[i] = world.CreateObject();
             objects[i]->AddComponent<Position>();
             objects[i]->AddComponent<Velocity>();
         }
         world.Update(0);
         bool wasTen = velocitySystem->objectCounter == 10;
         bool allComponentCountersWhereOne = true;
         for (int i=0;i<10;i++) {
             objects[i]->Remove();
             allComponentCountersWhereOne = allComponentCountersWhereOne && objects[i]->GetComponent<Position>()->counter == 1;
         }
         world.Update(0);
         bool isZero = velocitySystem->objectCounter == 0;
         
         return
         wasTen && isZero && allComponentCountersWhereOne;
    });
    
    UnitTest("Add/Remove Object from GameSystem::ObjectAdded", []()->bool {
        
        struct Bomb { int damage; };
        
        struct BombThrower {
            BombThrower() { numberOfBombsToCreate = 5; }
            int numberOfBombsToCreate;
        };
        
        struct BombSystem : public GameSystem<Bomb> {
            GameWorld* world;
            void Initialize(GameWorld* world) {
                this->world = world;
            }
        
            void ObjectAdded(GameObject* object) {bombs++;}
            void ObjectRemoved(GameObject* object) {bombs--;}
            int bombs = 0;
        };
        
        struct BombThrowerSystem : public GameSystem<BombThrower> {
            GameWorld* world;
            void Initialize(GameWorld* world) {
                this->world = world;
            }
            void ObjectAdded(GameObject* object) {
                for (int i=0; i<object->GetComponent<BombThrower>()->numberOfBombsToCreate; i++) {
                    GameObject* bomb = world->CreateObject();
                    bomb->AddComponent<Bomb>();
                }
                object->Remove();
            }
        };
        
        GameWorld world;
        world.CreateSystem<BombThrowerSystem>();
        auto bombSystem = world.CreateSystem<BombSystem>();
        GameObject* bombThrower = world.CreateObject();
        bombThrower->AddComponent<BombThrower>();
        
        assert(bombThrower->GetComponent<BombThrower>());
        

        size_t objectsBefore = world.ObjectCount();
        int bombsBefore = bombSystem->bombs;
        world.Update(0);
        size_t objectsAfter = world.ObjectCount();
        int bombsAfter = bombSystem->bombs;
        
        return objectsBefore == 1 && bombsBefore == 0 && objectsAfter == 5 && bombsAfter == 5;
    });
    
    
    UnitTest("Add same component multiple times same frame", []()->bool {
        struct Data {
            int something;
        };
        struct DataSystem : GameSystem<Data> {};
        GameWorld world;
        world.CreateSystem<DataSystem>();
        GameObject* object = world.CreateObject();
        auto c1 = object->AddComponent<Data>();
        auto c2 = object->AddComponent<Data>();
        return c1!=0 && c2!=0 && c1 == c2;
    });
    
    UnitTest("AddComponent() and AddComponent(ref) multiple times same frame", []()->bool {
        struct Data {
            Data() { something = 1; }
            int something;
        };
        struct DataSystem : GameSystem<Data> {};
        GameWorld world;
        world.CreateSystem<DataSystem>();
        GameObject* ref = world.CreateObject();
        ref->AddComponent<Data>()->something = 128;
        GameObject* object = world.CreateObject();
        auto c1 = object->AddComponent<Data>();
        auto c2 = object->AddComponent<Data>(ref);
        return c1!=0 && c2!=0 && c1 == c2 && c2->something == 1;
    });
    
    UnitTest("reference component", []()->bool {
        struct Data {
            int something;
        };
        struct DataSystem : GameSystem<Data> {};
        GameWorld world;
        world.CreateSystem<DataSystem>();
        GameObject* ref = world.CreateObject();
        ref->AddComponent<Data>()->something = 128;
        GameObject* o1 = world.CreateObject();
        GameObject* o2 = world.CreateObject();
        auto c1 = o1->AddComponent<Data>(ref);
        auto c2 = o2->AddComponent<Data>(ref);
        
        return c1!=0 && c2!=0 && c1 == c2 && c1->something == 128;
    });
    
    UnitTest("reference component vs non reference component", []()->bool {
        struct Data {
            Data() { something = 5; }
            int something;
        };
         struct DataSystem : GameSystem<Data> {};
        GameWorld world;
        world.CreateSystem<DataSystem>();
        GameObject* ref = world.CreateObject();
        ref->AddComponent<Data>()->something = 128;
        GameObject* o1 = world.CreateObject();
        GameObject* o2 = world.CreateObject();
        auto c1 = o1->AddComponent<Data>();
        auto c2 = o2->AddComponent<Data>(ref);
        ref->AddComponent<Data>()->something = 64;
        
        return c1!=0 && c2!=0 && c1 != c2 && c1->something == 5 && c2->something == 64;
    });
    
    UnitTest("Cleanup", []()->bool {
      
        struct Data {
            int something;
        };
        
        struct DataSystem : public GameSystem<Data> {
            void ObjectAdded(GameObject* object) {(*objects)++;}
            void ObjectRemoved(GameObject* object) {(*objects)--;}
            int* objects;
        };
        
        int objects = 0;
        int preCount = 0;
        {
            GameWorld world;
            auto system = world.CreateSystem<DataSystem>();
            system->objects = &objects;
            for (int i=0; i<100; i++) {
                auto o = world.CreateObject();
                o->AddComponent<Data>();
            }
            world.Update(0);
            preCount = objects;
        }
        
        return objects == 0 && preCount == 100;
    });
    
    UnitTest("Add/Remove Component same frame", []()->bool {
        
        struct Component { };
        
        struct ComponentSystem : public GameSystem<Component> {
            void ObjectAdded(GameObject* object) {added++;}
            void ObjectRemoved(GameObject* object) {removed++;}
            int added = 0;
            int removed = 0;
        };
        
         GameWorld world;
         auto system = world.CreateSystem<ComponentSystem>();
         GameObject* object = world.CreateObject();
         object->AddComponent<Component>();
         object->RemoveComponent<Component>();
         object->AddComponent<Component>();
         object->RemoveComponent<Component>();
         world.Update(0);
         
         return system->added == 1 && system->removed == 1;
     });
    
     UnitTest("Parent/Children", []()->bool {
         
         GameWorld world;
         GameObject* mother = world.CreateObject();
         GameObject* child = world.CreateObject();
         child->Parent = mother;
         
         auto iterator = std::find(mother->Children().begin(), mother->Children().end(), child);
         return iterator != mother->Children().end() && mother->Children().size() == 1;
     });
    
    
    
    UnitTest("Parent/Children parent/unparent", []()->bool {
        
        GameWorld world;
        GameObject* mother = world.CreateObject();
        GameObject* child = world.CreateObject();
        child->Parent = mother;
        auto iterator = std::find(mother->Children().begin(), mother->Children().end(), child);
        bool wasChildOfMother = iterator != mother->Children().end();
        child->Parent = 0;
        iterator = std::find(mother->Children().begin(), mother->Children().end(), child);
        bool isOrphaned = iterator == mother->Children().end();
        return wasChildOfMother && isOrphaned && mother->Children().size() == 0;
    });
    
    
    UnitTest("Parent/Children hierarchal remove", []()->bool {
        struct Data {
            int something;
        };
        
        struct DataSystem : public GameSystem<Data> {
            void ObjectAdded(GameObject* object) { added++; }
            void ObjectRemoved(GameObject* object) { removed++; }
            int added;
            int removed;
        };
        
        
        GameWorld world;
        auto system = world.CreateSystem<DataSystem>();
        GameObject* mother = world.CreateObject();
        mother->AddComponent<Data>();
        GameObject* child = world.CreateObject();
        child->AddComponent<Data>();
        child->Parent = mother;
        world.Update(0);
        bool firstTest = system->added == 2 && system->removed == 0;
        mother->Remove();
        world.Update(0);
        bool secondTest = system->added == 2 && system->removed == 2;
             
        return firstTest && secondTest;
    });
    
    UnitTest("Clone object", []()->bool {
        GameWorld world;
        GameObject* g1 = world.CreateObject();
        GameObject* g2 = g1;//g1->Clone();
        return g1!=g2 && world.ObjectCount() == 2;
    });
    
    
    UnitTest("Hierarchal clone object", []()->bool {
        GameWorld world;
        GameObject* g1 = world.CreateObject();
        GameObject* child = world.CreateObject();
        child->Parent = g1;
        GameObject* g2 = g1;//g1->Clone();
        return g1!=g2 && world.ObjectCount() == 4 && g2->Children()[0] != 0;
    });
    
    
    UnitTest("Clone object with component", []()->bool {
    
        struct Data {
            std::string name;
            int age;
        };
        struct DataSystem : GameSystem<Data> {};
        GameWorld world;
        world.CreateSystem<DataSystem>();
        GameObject* o1 = world.CreateObject();
        o1->AddComponent<Data>()->name = "Igor";
        o1->GetComponent<Data>()->age = 32;
        
        GameObject* o2 = o1;//o1->Clone();
        
        return
        o1!=o2 &&
        o1->GetComponent<Data>()!=o2->GetComponent<Data>() &&
        o1->GetComponent<Data>()->name == o2->GetComponent<Data>()->name &&
        o1->GetComponent<Data>()->age == o2->GetComponent<Data>()->age;
    });
    
     UnitTest("Clone component", []()->bool {
    
        struct Data {
            std::string name;
            int age;
        };
    
        struct DataSystem : GameSystem<Data> {};
        GameWorld world;
        world.CreateSystem<DataSystem>();
        GameObject* o1 = world.CreateObject();
        o1->AddComponent<Data>()->name = "Igor";
        o1->GetComponent<Data>()->age = 32;
        
        GameObject* o2 = world.CreateObject();
        //o2->CloneComponent<Data>(o1);
        /*
        return
        o1!=o2 &&
        o1->GetComponent<Data>()!=o2->GetComponent<Data>() &&
        o1->GetComponent<Data>()->name == o2->GetComponent<Data>()->name &&
        o1->GetComponent<Data>()->age == o2->GetComponent<Data>()->age;
        */
        return false;
    });
    
    UnitTest("Clone reference component", []()->bool {
    
        struct Texture {
            int Color;
        };
        
        struct TextureSystem : GameSystem<Texture> {};
        GameWorld world;
        world.CreateSystem<TextureSystem>();
        GameObject* texture = world.CreateObject();
        texture->AddComponent<Texture>()->Color = 456;
        
        GameObject* referenceTexture = world.CreateObject();
        referenceTexture->AddComponent<Texture>(texture);
        
        GameObject* clone = 0;//referenceTexture->Clone();
        /*
        GameObject* textureCopy = world.CreateObject();
        textureCopy->CloneComponent<Texture>(texture);
        
        GameObject* textureComponentRef = world.CreateObject();
        textureComponentRef->CloneComponent<Texture>(referenceTexture);
        
        GameObject* copied = textureCopy->Clone();
        
        bool first = referenceTexture->GetComponent<Texture>() == texture->GetComponent<Texture>();
        bool second = clone->GetComponent<Texture>() == texture->GetComponent<Texture>();
        bool third = copied->GetComponent<Texture>() != textureCopy->GetComponent<Texture>() &&
                     copied->GetComponent<Texture>() != referenceTexture->GetComponent<Texture>();
        bool fourth = referenceTexture->GetComponent<Texture>() == textureComponentRef->GetComponent<Texture>();
        return first && second && third && fourth;
        */
        return false;
    });
    
    
    UnitTest("IsComponentReference<T>", []()->bool {
        struct ExpensiveStuff {
            int HugeData;
        };
        
        struct ExpensiveStuffSystem : GameSystem<ExpensiveStuff> {};
        GameWorld world;
        world.CreateSystem<ExpensiveStuffSystem>();
        GameObject* database = world.CreateObject();
        database->AddComponent<ExpensiveStuff>()->HugeData = 100;
        
        GameObject* refObject = world.CreateObject();
        refObject->AddComponent<ExpensiveStuff>(database);
        
        //return !database->IsComponentReference<ExpensiveStuff>() && refObject->IsComponentReference<ExpensiveStuff>();
        return false;
    });
    
     UnitTest("cloned IsComponentReference<T>", []()->bool {
        struct ExpensiveStuff {
            int HugeData;
        };
        
        struct ExpensiveStuffSystem : GameSystem<ExpensiveStuff> {};
        GameWorld world;
        world.CreateSystem<ExpensiveStuffSystem>();
        GameObject* database = world.CreateObject();
        database->AddComponent<ExpensiveStuff>()->HugeData = 100;
        
        GameObject* refObject = world.CreateObject();
        refObject->AddComponent<ExpensiveStuff>(database);
        /*
        GameObject* cloneRef = refObject->Clone();
        GameObject* cloneDatabase = database->Clone();
        
        return cloneRef->IsComponentReference<ExpensiveStuff>() && !cloneDatabase->IsComponentReference<ExpensiveStuff>()
                &&
                refObject->GetComponent<ExpensiveStuff>() == database->GetComponent<ExpensiveStuff>()
                &&
                cloneRef->GetComponent<ExpensiveStuff>() == database->GetComponent<ExpensiveStuff>()
                &&
                cloneDatabase->GetComponent<ExpensiveStuff>()!=database->GetComponent<ExpensiveStuff>();
                */
                return false;
    });
    
    UnitTest("cloned object System ObjectAdded/Removed", [] () ->bool{
        struct Nameable {
            std::string name;
        };
        struct Moveable {
            float speed;
        };
        struct Mover : public GameSystem<Nameable, Moveable> {
            void ObjectAdded(GameObject* object) {count++;}
            void ObjectRemoved(GameObject* object) {count--;}
        public:
            int count = 0;
        };
        /*
        GameWorld world;
        world.Initialize<Mover>();
        Mover* system = world.GetSystem<Mover>();
        
        GameObject* person = world.CreateObject();
        person->AddComponent<Nameable>()->name = "Albert";
        person->AddComponent<Moveable>()->speed = 10;
        
        world.Update(0);
        bool countWasOne = system->count == 1;
        
        GameObject* clone = person->Clone();
        world.Update(0);
        
        bool wasTwo = system->count == 2;
        
        person->Remove();
        
        world.Update(0);
        bool isOneLeft = world.Objects().size() == 1 && system->count == 1;
    
        clone->Remove();
        world.Update(0);
        bool isNoneLeft = world.Objects().size() == 0 && system->count == 0;
        return countWasOne && wasTwo && isOneLeft && isNoneLeft;
        */
        return false;
    });


    UnitTest("Clone component with Property", [] () {
    
        struct PropertyComponent {
            public:
                PropertyComponent() { Value = 0;}
                Property<int> Value;
        };
    
        struct PropertyComponentSystem : GameSystem<PropertyComponent> {};
        GameWorld world;
        world.CreateSystem<PropertyComponentSystem>();
        
        GameObject* source = world.CreateObject();
        PropertyComponent* property1 = source->AddComponent<PropertyComponent>();
        property1->Value = 123;
        /*
        GameObject* clone = source->Clone();
        PropertyComponent* property2 = clone->GetComponent<PropertyComponent>();
        
        return property1->Value == 123 && property2->Value == 123 && property1!=property2;
        */
        return false;
    });


    UnitTest("Test pointer to child, cloned", [] () {
        /*Component(PointerToParent)
            Pointer<GameObject> MyParent;
        };
        
        GameWorld world;
        GameObject* object = world.CreateObject();
        GameObject* child = world.CreateObject();
        child->Parent = object;
        child->AddComponent<PointerToParent>()->MyParent = object;
        
        GameObject* clone = object->Clone();
        GameObject* cloneChild = clone->Children()[0];
        
        GameObject* childParent = child->GetComponent<PointerToParent>()->MyParent;
        GameObject* cloneChildParent = cloneChild->GetComponent<PointerToParent>()->MyParent;
    
        return object!=clone &&
        child!=cloneChild &&
        childParent!=cloneChildParent &&
        child->Parent == object &&
        cloneChild->Parent == clone &&
        childParent == object &&
        cloneChildParent == clone;
        */
        return false;
    });

    UnitTest("Test pointer to removed object", [] () -> bool {
        /*
        Component(PointerComponent)
            Pointer<GameObject> GameObject;
        };
        
        GameWorld world;
        GameObject* gameObject = world.CreateObject();
    
        GameObject* pointerObject = world.CreateObject();
        pointerObject->AddComponent<PointerComponent>()->GameObject = gameObject;
        
        GameObject* oldPointer = pointerObject->GetComponent<PointerComponent>()->GameObject;
        bool wasPointingToObject = oldPointer==gameObject;
        gameObject->Remove();
        world.Update(0);
        GameObject* newPointer = pointerObject->GetComponent<PointerComponent>()->GameObject;
        bool isPointingToNothing = newPointer == 0;
        return wasPointingToObject && isPointingToNothing;
        */
        return false;
    });
    
    UnitTest("Test pointer to removed object with cloned object", [] () -> bool {
        /*
        Component(PointerComponent)
            Pointer<GameObject> GameObject;
        };
        
        GameWorld world;
        GameObject* gameObject = world.CreateObject();
    
        GameObject* pointerObject = world.CreateObject();
        pointerObject->AddComponent<PointerComponent>()->GameObject = gameObject;
        
        GameObject* clonedObject = pointerObject->Clone();
        
        GameObject* oldPointer = pointerObject->GetComponent<PointerComponent>()->GameObject;
        bool wasPointingToObject = oldPointer==gameObject;
        
        GameObject* oldPointerCloned = clonedObject->GetComponent<PointerComponent>()->GameObject;
        bool wasPointingToObjectCloned = oldPointerCloned==gameObject;
        
        gameObject->Remove();
        world.Update(0);
        GameObject* newPointer = pointerObject->GetComponent<PointerComponent>()->GameObject;
        GameObject* newPointerCloned = clonedObject->GetComponent<PointerComponent>()->GameObject;
        
        return wasPointingToObject && wasPointingToObjectCloned && !newPointer && !newPointerCloned;
        */
        return false;
    });
    
    UnitTest("Test pointer to object with removed cloned object", [] () -> bool {
        /*
        Component(PointerComponent)
            Pointer<GameObject> GameObject;
        };
        
        GameWorld world;
        GameObject* gameObject = world.CreateObject();
        
        
        GameObject* pointerObject = world.CreateObject();
        pointerObject->AddComponent<PointerComponent>()->GameObject = gameObject;
        
        GameObject* clonedObject = pointerObject->Clone();
        
        GameObject* oldPointer = pointerObject->GetComponent<PointerComponent>()->GameObject;
        bool wasPointingToObject = oldPointer==gameObject;
        
        GameObject* oldPointerCloned = clonedObject->GetComponent<PointerComponent>()->GameObject;
        bool wasPointingToObjectCloned = oldPointerCloned==gameObject;
        
        GameObject* clonedGameObject = gameObject->Clone();
        clonedGameObject->Remove();
        world.Update(0);
        GameObject* newPointer = pointerObject->GetComponent<PointerComponent>()->GameObject;
        GameObject* newPointerCloned = clonedObject->GetComponent<PointerComponent>()->GameObject;
        
        return wasPointingToObject && wasPointingToObjectCloned && newPointer==gameObject && newPointerCloned==gameObject;
        */
        return false;
    });
    
    
    UnitTest("Test pointer to component, cloned", [] () {
        /*
        Component(Moveable)
            float speed;
        };
        Component(Mover)
            Pointer<Moveable> Moveable;
        };
        
        GameWorld world;
        GameObject* object = world.CreateObject();
        object->AddComponent<Moveable>();
        object->AddComponent<Mover>()->Moveable = object->GetComponent<Moveable>();
        
        GameObject* clone = object->Clone();
        
        return
        object!=clone &&
        object->GetComponent<Mover>()->Moveable == object->GetComponent<Moveable>() &&
        clone->GetComponent<Mover>()->Moveable == clone->GetComponent<Moveable>() &&
        clone->GetComponent<Mover>()->Moveable != object->GetComponent<Moveable>();
        */
        return false;
    });


    UnitTest("Test pointer to component, cloned, with reference component", [] () {
        /*
        Component(Moveable)
            float speed;
        };
        Component(Mover)
            Pointer<Moveable> Moveable;
        };
        
        GameWorld world;
        GameObject* object = world.CreateObject();
        object->AddComponent<Moveable>();
        
        GameObject* moverObject = world.CreateObject();
        moverObject->AddComponent<Mover>()->Moveable = object->GetComponent<Moveable>();
        
        object->AddComponent<Mover>(moverObject);
        GameObject* clone = object->Clone();
        
        return
        object!=clone &&
        object->GetComponent<Mover>()->Moveable == object->GetComponent<Moveable>() &&
        clone->GetComponent<Mover>()->Moveable == object->GetComponent<Moveable>() &&
        object->GetComponent<Mover>() == moverObject->GetComponent<Mover>() &&
        clone->GetComponent<Mover>() == moverObject->GetComponent<Mover>();
        */
        return false;
    });
    
   UnitTest("Test creation and deletion in System::ObjectAdded/Removed", [] () {
        struct Parentable {
            GameObject* child;
        };
        struct ParentSystem : public GameSystem<Parentable> {
            GameWorld* world;
            void Initialize(GameWorld* world) {
                this->world = world;
            }
            void ObjectAdded(GameObject* object) {
                GameObject* child = world->CreateObject();
                child->Parent = object;
                object->AddComponent<Parentable>()->child = child;
            }
            void ObjectRemoved(GameObject* object) {
                object->GetComponent<Parentable>()->child->Remove();
            }
        };
        
        GameWorld world;
        world.CreateSystem<ParentSystem>();
        GameObject* object = world.CreateObject();
        object->AddComponent<Parentable>();
        world.Update(0);
        bool wasTwoObjectsCreated = world.ObjectCount() == 2;
        bool childWasCreated = object->GetComponent<Parentable>()->child == world.GetObject(1);
        object->Remove();
        world.Update(0);
        bool noneLeft = world.ObjectCount() == 0;
        return
        wasTwoObjectsCreated && childWasCreated && noneLeft;
    });
    
     UnitTest("Test double deletion of GameObject", [] () {
        GameWorld world;
        GameObject* object = world.CreateObject();
        world.Update(0);
        bool wasObjectCreated = world.ObjectCount() == 1;
        object->Remove();
        world.Update(0);
        bool wasObjectRemoved = world.ObjectCount() == 0;
        object->Remove();
        world.Update(0);
        object->Remove();
        bool wasObjectRemoved2 = world.ObjectCount() == 0;
        return
        wasObjectCreated && wasObjectRemoved && wasObjectRemoved2;
    });
    
    UnitTest("Delete and create object, test hierarchy", [] () {
        GameWorld world;
        GameObject* object = world.CreateObject();
        GameObject* child = world.CreateObject();
        child->Parent = object;
        world.Update(0);
        bool wasObjectCreated = world.ObjectCount() == 2;
        bool childrenCountIsOne = world.ObjectCount() == 1;
        object->Remove();
        world.Update(0);
        
        bool wasObjectRemoved = world.ObjectCount() == 0;
        bool childrenCountWasZero = world.ObjectCount() == 0;
        object = world.CreateObject();
        child = world.CreateObject();
        child->Parent = object;
        //object->Clone();
        world.Update(0);
        bool wasObjectCreated2 = world.ObjectCount() == 4;
        bool childrenCountIsOne2 = world.ObjectCount() == 2;
        
        return wasObjectCreated && childrenCountIsOne && wasObjectRemoved && childrenCountWasZero && wasObjectCreated2 && childrenCountIsOne2;
    });


}