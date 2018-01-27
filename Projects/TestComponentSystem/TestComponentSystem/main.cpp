//
//  main.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 07/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "LogicTests.hpp"
#include "ScriptTests.hpp"
#include "SerializationTests.hpp"
#include "GameObjectJsonSerializer.hpp"
#include "GameStorage.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

struct Transform {
    Transform() : x(0), y(0) {}

    int x;
    int y;

    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Nameable {
    std::string name;

    TYPE_FIELDS_BEGIN
    TYPE_FIELD(name)
    TYPE_FIELDS_END
};



int main(int argc, const char * argv[]) {

    std::cout << "sizeof(GameObject) = "<< sizeof(GameObject) << std::endl;

    std::cout <<std::endl << "Logic Tests:"<<std::endl;

    LogicTests tests;
    tests.Run();

    std::cout <<std::endl << "Serialization Tests:"<<std::endl;

    SerializationTests serializationTests;
    serializationTests.Run();
    
    std::cout <<std::endl << "Script Tests:"<<std::endl;
    
    ScriptTests scriptTests;
    scriptTests.Run();
    
//    GameStorage storage;
//    storage.AddComponentType<Transform>();
//    storage.AddComponentType<Nameable>();
//    storage.CreateSerializer<GameObjectJsonSerializer>();
//
//    GameWorld world(storage);
//
//    auto root = world.CreateScene();
//    root->AddComponent<Transform>()->x = 123;
//    root->AddComponent<Nameable>()->name = "Jeppe";
//
//    std::stringstream ss;
//    serializer.Serialize(root, ss);
//
//    std::cout << ss.str() << std::endl;
//
//    auto newObject = serializer.Deserialize(root, ss);
//
//    serializer.Serialize(root, std::cout, nullptr);
    

    /*
    ScriptTests scriptTests;
    scriptTests.Run();
    */
    return 0;
}
