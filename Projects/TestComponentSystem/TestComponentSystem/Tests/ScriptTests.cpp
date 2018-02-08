//
//  ScriptTests.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 23/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptTests.hpp"
#include "GameStorage.hpp"
#include "GameWorld.hpp"
#include "ScriptWorld.hpp"
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

TypeEditorTitle::Callback TypeEditorTitle::Title = [] (void* guiPtr, void* parentPtr, const std::string& title) -> void* {
    
    return 0;
};

void ScriptTests::RunTests() {

/*
    AddTest("Script compiling", [] () {
    
        std::vector<std::string> defaultIncludes =
        {
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Data/Timeline.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector2.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Rendering/Mesh.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/VertexMesh.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/TextureAtlas.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/Colour.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Interaction/Touchable.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Input/InputController.hpp"
        };
    
        ScriptWorld scriptWorld;
        scriptWorld.SetClangSdkPath("/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/");
        
        defaultIncludes.push_back("/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Scripts/ScriptTest.hpp");
        
        scriptWorld.SetFiles(
            "ScriptExample.so",
            "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/ScriptInclude",
            { "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Scripts/ScriptTest.cpp" },
            defaultIncludes
        );
        
        GameStorage storage;
        GameWorld world(storage);
        scriptWorld.SetStorage(storage);
        
        scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a", [] (auto& error) {});
        scriptWorld.LoadLib();
        
        scriptWorld.AddStorage(storage);
        //scriptWorld.RemoveStorage(storage);
        
        GameObject* root = world.CreateScene();
        
        root->AddComponent(1);
        root->AddComponent(2);
        
        GameObject* root2 = world.CreateScene();
        root2->AddComponent(1);
        root2->AddComponent(2);
        
        auto types = root2->GetComponentTypes([](int i) {
            return true;
        });

        {
            minijson::writer_configuration config;
            config = config.pretty_printing(true);
            minijson::object_writer writer(std::cout, config);
            
            types[0].Serialize(writer);
            types[1].Serialize(writer);
            
            writer.close();
        }
    
        world.Update(1.0f);
        
        {
            minijson::writer_configuration config;
            config = config.pretty_printing(true);
            minijson::object_writer writer(std::cout, config);
            
            types[0].Serialize(writer);
            types[1].Serialize(writer);
            
            writer.close();
        }
        
        GameObjectJsonSerializer serializer;
        serializer.Serialize(root, std::cout);
        
        auto fieldType = types[2].GetField("velocity");
        FieldInfo<int>* info = (FieldInfo<int>*)fieldType.get();
        
        return (*info->field)==456;
    });
    */
    
    AddTest("Script recompiling", [] () {
    
        std::vector<std::string> defaultIncludes =
        {
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Data/Timeline.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector2.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Rendering/Mesh.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/VertexMesh.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/TextureAtlas.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/Colour.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Interaction/Touchable.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Input/InputController.hpp"
        };
    
        ScriptWorld scriptWorld;
        scriptWorld.SetClangSdkPath("/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/");
        
        defaultIncludes.push_back("/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Scripts/ScriptTestRecompile1.hpp");
        
        scriptWorld.SetFiles(
            "ScriptExample.so",
            "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/ScriptInclude",
            { "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Scripts/ScriptTestRecompile1.cpp" },
            defaultIncludes
        );
        
        GameStorage storage;
        storage.CreateSerializer<GameObjectJsonSerializer>();
        GameWorld world(storage);
        scriptWorld.SetStorage(storage);
        
        scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a", [] (auto& error) {});
        scriptWorld.LoadLib();
        
        scriptWorld.AddStorage(storage);
        
        GameObject* root = world.CreateScene();
        
        root->AddComponent(1);
        root->AddComponent(2);
        
        
        bool firstTest = false;
        {
            auto types = root->GetComponentTypes([](int i) {
                return true;
            });
        
            auto fieldType = types[2].GetField("velocity");
            FieldInfo<int>* info = (FieldInfo<int>*)fieldType.get();
            
            firstTest = (*info->field)==456;
            (*info->field) = 666;
        }
        
        std::stringstream savedComponents;
        storage.SerializeAndRemoveComponents(savedComponents, [](const GameObject* o, int componentId) {
            return componentId>0;
        });
       
        std::cout << savedComponents.str() << std::endl;
        
        scriptWorld.RemoveStorage(storage);
        scriptWorld.UnloadLib();
        
        scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a", [] (auto& error) {});
        scriptWorld.LoadLib();
        
        scriptWorld.AddStorage(storage);
        storage.DeserializeAndAddComponents(savedComponents);
        
        
        bool secondTest = false;
        {
            auto types = root->GetComponentTypes([](int i) {
                return true;
            });
        
            auto fieldType = types[2].GetField("velocity");
            FieldInfo<int>* info = (FieldInfo<int>*)fieldType.get();
            
            secondTest = (*info->field) == 666;
        }
        
        return firstTest && secondTest;
    });
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
