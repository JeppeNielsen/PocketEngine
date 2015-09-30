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
        return obj1 && obj2 && obj1!=obj2 && world.Objects().size()==2;
    });
    
    UnitTest("Delete all objects", []()->bool {
        GameWorld world;
        for (int i=0; i<100; i++) {
            world.CreateObject();
        }
        bool wasHundred = world.Objects().size()==100;
        world.DeleteAllObjects();
        return world.Objects().empty() && wasHundred;
    });
    
    UnitTest("Create Component", []()->bool {
        Component(Position)
            int Position;
            void Reset() { Position = 5; }
        };
        
        GameWorld world;
        auto object = world.CreateObject();
        auto position = object->AddComponent<Position>();
        return position!=0;
    });
    
    UnitTest("Reset Component", []()->bool {
        Component(Position)
        public:
            int Position;
            void Reset() { Position = 5; }
        };
        GameWorld world;
        auto object = world.CreateObject();
        auto position = object->AddComponent<Position>();
        return position->Position==5;
    });
    
    UnitTest("GetComponent", []()->bool {
        Component(Position)
        public:
            int Position;
        };
        GameWorld world;
        auto object = world.CreateObject();
        object->AddComponent<Position>();
        return object->GetComponent<Position>()!=0;
    });

    
    UnitTest("RemoveComponent", []()->bool {
        Component(Position)
        public:
            int Position;
            void Reset() { Position = 5; }
        };
        GameWorld world;
        auto object = world.CreateObject();
        bool componentDidExist = object->AddComponent<Position>()!=0;
        world.Update(0);
        object->RemoveComponent<Position>();
        world.Update(0);
        return componentDidExist && object->GetComponent<Position>()==0;
    });
    
    UnitTest("EnableComponent", []()->bool {
        Component(Position)
        public:
            int Position;
            void Reset() { Position = 5; }
        };
        GameWorld world;
        auto object = world.CreateObject();
        object->AddComponent<Position>();
        world.Update(0);
        bool wasEnabled = object->IsComponentEnabled<Position>();
        object->EnableComponent<Position>(false);
        world.Update(0);
        bool isEnabled = object->IsComponentEnabled<Position>();
        world.Update(0);
        return wasEnabled && !isEnabled;
    });
    
    UnitTest("Add System", []()->bool {
        Component(Position)
        public:
            int counter;
            int Position;
            void Reset() { counter = 0; Position = 5; }
        };
        
        Component(Velocity)
        public:
            int Velocity;
            void Reset() { Velocity = 5; }
        };
        SYSTEM(VelocitySystem, Position, Velocity)
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
        Component(Position)
        public:
            int counter;
            int Position;
            void Reset() { counter = 0; Position = 5; }
        };
        
        Component(Velocity)
        public:
            int Velocity;
            void Reset() { Velocity = 5; }
        };
        SYSTEM(VelocitySystem, Position, Velocity)
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
        Component(Position)
        public:
            int counter;
            int Position;
            void Reset() { counter = 0; Position = 5; }
        };
        
        Component(Velocity)
        public:
            int Velocity;
            void Reset() { Velocity = 5; }
        };
        
        SYSTEM(VelocitySystem, Position, Velocity)
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
        
        Component(Bomb)
        public:
            int damage;
        };
        
        Component(BombThrower)
        public:
            int numberOfBombsToCreate;
            void Reset() { numberOfBombsToCreate = 5; }
        };
        
        SYSTEM(BombSystem, Bomb)
            void ObjectAdded(GameObject* object) {bombs++;}
            void ObjectRemoved(GameObject* object) {bombs--;}
            public:
            int bombs = 0;
        };
        
        SYSTEM(BombThrowerSystem, BombThrower)
        void ObjectAdded(GameObject* object) {
            for (int i=0; i<object->GetComponent<BombThrower>()->numberOfBombsToCreate; i++) {
                GameObject* bomb = World()->CreateObject();
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
             
             size_t objectsBefore = world.Objects().size();
             int bombsBefore = bombSystem->bombs;
             world.Update(0);
             size_t objectsAfter = world.Objects().size();
             int bombsAfter = bombSystem->bombs;
             
        
        return objectsBefore == 1 && bombsBefore == 0 && objectsAfter == 5 && bombsAfter == 5;
    });
    
    UnitTest("Add same component multiple times same frame", []()->bool {
        Component(Data)
        public:
            int something;
        };
        GameWorld world;
        GameObject* object = world.CreateObject();
        auto c1 = object->AddComponent<Data>();
        auto c2 = object->AddComponent<Data>();
        return c1!=0 && c2!=0 && c1 == c2;
    });
    
    UnitTest("AddComponent() and AddComponent(ref) multiple times same frame", []()->bool {
        Component(Data)
        public:
            void Reset() { something = 1; }
            int something;
        };
        GameWorld world;
        GameObject* ref = world.CreateObject();
        ref->AddComponent<Data>()->something = 128;
        GameObject* object = world.CreateObject();
        auto c1 = object->AddComponent<Data>();
        auto c2 = object->AddComponent<Data>(ref);
        return c1!=0 && c2!=0 && c1 == c2 && c2->something == 1;
    });
    
    UnitTest("reference component", []()->bool {
        Component(Data)
        public:
            int something;
        };
        GameWorld world;
        GameObject* ref = world.CreateObject();
        ref->AddComponent<Data>()->something = 128;
        GameObject* o1 = world.CreateObject();
        GameObject* o2 = world.CreateObject();
        auto c1 = o1->AddComponent<Data>(ref);
        auto c2 = o2->AddComponent<Data>(ref);
        
        return c1!=0 && c2!=0 && c1 == c2 && c1->something == 128;
    });
    
    UnitTest("reference component vs non reference component", []()->bool {
        Component(Data)
        public:
            int something;
            void Reset() { something = 5; }
        };
        GameWorld world;
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
      
        Component(Data)
        public:
            int something;
        };
        
        SYSTEM(DataSystem, Data)
            void ObjectAdded(GameObject* object) {(*objects)++;}
            void ObjectRemoved(GameObject* object) {(*objects)--;}
            public:
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
        
        Component(Component)
            public:
        };
        
        SYSTEM(ComponentSystem, Component)
            void ObjectAdded(GameObject* object) {added++;}
            void ObjectRemoved(GameObject* object) {removed++;}
            public:
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
        Component(Data)
        public:
            int something;
        };
        
        SYSTEM(DataSystem, Data)
            void ObjectAdded(GameObject* object) { added++; }
            void ObjectRemoved(GameObject* object) { removed++; }
            public:
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
        GameObject* g2 = g1->Clone();
        return g1!=g2 && world.Objects().size() == 2;
    });
    
    
    UnitTest("Hierarchal clone object", []()->bool {
        GameWorld world;
        GameObject* g1 = world.CreateObject();
        GameObject* child = world.CreateObject();
        child->Parent = g1;
        GameObject* g2 = g1->Clone();
        return g1!=g2 && world.Objects().size() == 4 && g2->Children()[0] != 0;
    });
    
    
    UnitTest("Clone object with component", []()->bool {
    
        Component(Data)
            public:
                std::string name;
                int age;
        };
    
        GameWorld world;
        GameObject* o1 = world.CreateObject();
        o1->AddComponent<Data>()->name = "Igor";
        o1->GetComponent<Data>()->age = 32;
        
        GameObject* o2 = o1->Clone();
        
        return
        o1!=o2 &&
        o1->GetComponent<Data>()!=o2->GetComponent<Data>() &&
        o1->GetComponent<Data>()->name == o2->GetComponent<Data>()->name &&
        o1->GetComponent<Data>()->age == o2->GetComponent<Data>()->age;
    });
    
     UnitTest("Clone component", []()->bool {
    
        Component(Data)
            public:
                std::string name;
                int age;
        };
    
        GameWorld world;
        GameObject* o1 = world.CreateObject();
        o1->AddComponent<Data>()->name = "Igor";
        o1->GetComponent<Data>()->age = 32;
        
        GameObject* o2 = world.CreateObject();
        o2->CloneComponent<Data>(o1);
        
        return
        o1!=o2 &&
        o1->GetComponent<Data>()!=o2->GetComponent<Data>() &&
        o1->GetComponent<Data>()->name == o2->GetComponent<Data>()->name &&
        o1->GetComponent<Data>()->age == o2->GetComponent<Data>()->age;
    });
    
    UnitTest("Clone reference component", []()->bool {
    
        Component(Texture)
            public:
                int Color;
        };
    
        GameWorld world;
        GameObject* texture = world.CreateObject();
        texture->AddComponent<Texture>()->Color = 456;
        
        GameObject* referenceTexture = world.CreateObject();
        referenceTexture->AddComponent<Texture>(texture);
        
        GameObject* clone = referenceTexture->Clone();
        
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
    });
    
    
    UnitTest("IsComponentReference<T>", []()->bool {
        Component(ExpensiveStuff)
            int HugeData;
        };
        
        GameWorld world;
        GameObject* database = world.CreateObject();
        database->AddComponent<ExpensiveStuff>()->HugeData = 100;
        
        GameObject* refObject = world.CreateObject();
        refObject->AddComponent<ExpensiveStuff>(database);
        
        return !database->IsComponentReference<ExpensiveStuff>() && refObject->IsComponentReference<ExpensiveStuff>();
    });
    
     UnitTest("cloned IsComponentReference<T>", []()->bool {
        Component(ExpensiveStuff)
            int HugeData;
        };
        
        GameWorld world;
        GameObject* database = world.CreateObject();
        database->AddComponent<ExpensiveStuff>()->HugeData = 100;
        
        GameObject* refObject = world.CreateObject();
        refObject->AddComponent<ExpensiveStuff>(database);
        
        GameObject* cloneRef = refObject->Clone();
        GameObject* cloneDatabase = database->Clone();
        
        return cloneRef->IsComponentReference<ExpensiveStuff>() && !cloneDatabase->IsComponentReference<ExpensiveStuff>()
                &&
                refObject->GetComponent<ExpensiveStuff>() == database->GetComponent<ExpensiveStuff>()
                &&
                cloneRef->GetComponent<ExpensiveStuff>() == database->GetComponent<ExpensiveStuff>()
                &&
                cloneDatabase->GetComponent<ExpensiveStuff>()!=database->GetComponent<ExpensiveStuff>();
    });
    
    UnitTest("cloned object System ObjectAdded/Removed", [] () {
        Component(Nameable)
            std::string name;
        };
        Component(Moveable)
            float speed;
        };
        SYSTEM(Mover, Nameable, Moveable)
            void ObjectAdded(GameObject* object) {count++;}
            void ObjectRemoved(GameObject* object) {count--;}
        public:
            int count = 0;
        };
        
        GameWorld world;
        Mover* system = world.CreateSystem<Mover>();
        
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
    });


    UnitTest("Clone component with Property", [] () {
    
        Component(PropertyComponent)
            public:
                PropertyComponent() : Value(this) {}
                Property<PropertyComponent*, int> Value;
        };
    
        GameWorld world;
        
        GameObject* source = world.CreateObject();
        PropertyComponent* property1 = source->AddComponent<PropertyComponent>();
        property1->Value = 123;
        
        GameObject* clone = source->Clone();
        PropertyComponent* property2 = clone->GetComponent<PropertyComponent>();
        
        return property1->Value == 123 && property2->Value == 123 && property1!=property2;
    });


    UnitTest("Test pointer to child, cloned", [] () {
        Component(PointerToParent)
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
    });

    UnitTest("Test pointer to removed object", [] () -> bool {
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
    });
    
    UnitTest("Test pointer to removed object with cloned object", [] () -> bool {
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
    });
    
    UnitTest("Test pointer to object with removed cloned object", [] () -> bool {
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
    });
    
    
    UnitTest("Test pointer to component, cloned", [] () {
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
    });


    UnitTest("Test pointer to component, cloned, with reference component", [] () {
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
    });
    
   UnitTest("Test creation and deletion in System::ObjectAdded/Removed", [] () {
        Component(Parentable)
            GameObject* child;
        };
        SYSTEM(ParentSystem, Parentable)
            void ObjectAdded(GameObject* object) {
                GameObject* child =World()->CreateObject();
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
        bool wasTwoObjectsCreated = world.Objects().size() == 2;
        bool childWasCreated = object->GetComponent<Parentable>()->child == world.Objects()[1];
        object->Remove();
        world.Update(0);
        bool noneLeft = world.Objects().empty();
        return
        wasTwoObjectsCreated && childWasCreated && noneLeft;
    });
    
     UnitTest("Test double deletion of GameObject", [] () {
        GameWorld world;
        GameObject* object = world.CreateObject();
        world.Update(0);
        bool wasObjectCreated = world.Objects().size() == 1;
        object->Remove();
        world.Update(0);
        bool wasObjectRemoved = world.Objects().size() == 0;
        object->Remove();
        world.Update(0);
        object->Remove();
        bool wasObjectRemoved2 = world.Objects().size() == 0;
        return
        wasObjectCreated && wasObjectRemoved && wasObjectRemoved2;
    });
    
    UnitTest("Delete and create object, test hierarchy", [] () {
        GameWorld world;
        GameObject* object = world.CreateObject();
        GameObject* child = world.CreateObject();
        child->Parent = object;
        world.Update(0);
        bool wasObjectCreated = world.Objects().size() == 2;
        bool childrenCountIsOne = world.Children().size() == 1;
        object->Remove();
        world.Update(0);
        
        bool wasObjectRemoved = world.Objects().size() == 0;
        bool childrenCountWasZero = world.Children().size() == 0;
        object = world.CreateObject();
        child = world.CreateObject();
        child->Parent = object;
        object->Clone();
        world.Update(0);
        bool wasObjectCreated2 = world.Objects().size() == 4;
        bool childrenCountIsOne2 = world.Children().size() == 2;
        
        return wasObjectCreated && childrenCountIsOne && wasObjectRemoved && childrenCountWasZero && wasObjectCreated2 && childrenCountIsOne2;
    });


}