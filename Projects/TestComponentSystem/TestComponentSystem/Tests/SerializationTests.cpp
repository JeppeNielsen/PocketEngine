//
//  SerializationTests.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 02/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "SerializationTests.hpp"
#include "GameSystem.hpp"
#include "GameObjectHandle.hpp"
#include "FileWorld.hpp"
#include "GameObjectJsonSerializer.hpp"
#include "Timer.hpp"

using namespace Pocket;

namespace SerializationTestsComponents {

struct Transform {
    int x;
    int y;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Velocity {
    int x;
    int y;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Renderable {
    int imageNo;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(imageNo)
    TYPE_FIELDS_END
};

struct Mesh {
    std::vector<int> vertices;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(vertices)
    TYPE_FIELDS_END
};

struct Cloner {
    GameObjectHandle Source;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(Source)
    TYPE_FIELDS_END
};

struct Nameable {
    std::string name;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(name)
    TYPE_FIELDS_END
};

struct VelocitySystem : public GameSystem<Transform, Velocity> {

};

struct RenderSystem : public GameSystem<Transform, Mesh, Renderable> {

};

struct ClonerSystem : public GameSystem<Cloner> {

};

}

using namespace SerializationTestsComponents;

void LogStream(const std::string& text, const std::stringstream& s) {
    std::cout << text << ":" <<std::endl;
    std::cout << s.str() << std::endl<<std::endl;
}

void SerializationTests::RunTests() {

    const static std::string prefabGUID = "V7ey7J4UTkmvxHyiEWPRhA==";

    storage.AddSystemType<VelocitySystem>();
    storage.AddSystemType<RenderSystem>();
    storage.AddSystemType<ClonerSystem>();
    storage.AddComponentType<Nameable>();
    storage.CreateSerializer<GameObjectJsonSerializer>();

    fileWorld.Initialize(storage);
    fileWorld.FindRoots("/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Assets/", {"json"});


    AddTest("Try get prefab from Storage", [this] {
        GameObject* prefab = storage.TryGetPrefab(prefabGUID, 1);
        return prefab;
    });
    
    AddTest("Prefab has 1 child", [this] {
        GameObject* prefab = storage.TryGetPrefab(prefabGUID, 1);
        return prefab->Hierarchy().Children().size() == 1;
    });
    
    AddTest("Create scene from prefab", [this] {
        GameObject* prefab = storage.TryGetPrefab(prefabGUID, 1);
        GameWorld world(storage);
        auto scene = world.CreateScene(prefab);
        return scene!=prefab && scene;
    });
    
    AddTest("Scene from prefab has one child", [this] {
        GameObject* prefab = storage.TryGetPrefab(prefabGUID, 1);
        GameWorld world(storage);
        auto scene = world.CreateScene(prefab);
        return scene->Hierarchy().Children().size() == 1;
    });
    
    AddTest("Scene from prefab has same data", [this] {
        GameObject* prefab = storage.TryGetPrefab(prefabGUID, 1);
        GameWorld world(storage);
        auto scene = world.CreateScene(prefab);
        
        return prefab->GetComponent<Transform>()->x == scene->GetComponent<Transform>()->x &&
               prefab->GetComponent<Transform>()!=scene->GetComponent<Transform>();
    });
    
    AddTest("Json serializer", [this] {
        GameWorld world(storage);
        auto scene = world.CreateScene();
        GameObject* object = scene->CreateObject();
        object->AddComponent<Transform>()->x = 456;
        
        GameObjectJsonSerializer serializer;
        
        std::stringstream ss;
        serializer.Serialize(object, ss);
        
        GameObject* clone = scene->CreateObject();
        serializer.Deserialize(clone, ss);
        
        int x1 = object->GetComponent<Transform>()->x;
        int x2 = clone->GetComponent<Transform>()->x;
        
        return clone->GetComponent<Transform>()!=object->GetComponent<Transform>() &&
                 x1 == x2;
    });

    
    AddTest("Other stuff", [this] {
    
        Timer timer;
        timer.Begin();
        storage.TryGetPrefab(prefabGUID, 1);
        std::cout << "Time 1 : "<< timer.End() << std::endl;
        
        GameWorld world(storage);
        GameObject* scene = world.CreateScene();
        
        timer.Begin();
        for (int i=0; i<1000000; i++) {
            scene->CreateChild();
        }
        std::cout << "1.000.000 children : "<< timer.End() << std::endl;
        
        return true;
    });
}

