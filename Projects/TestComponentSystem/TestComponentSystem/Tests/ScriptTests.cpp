//
//  ScriptTests.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 23/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptTests.hpp"
#include "GameWorld.hpp"
#include "ScriptWorld.hpp"

using namespace Pocket;

void ScriptTests::RunTests() {

    AddTest("Script compiling", [] () {
    
        std::vector<std::string> defaultIncludes =
        {
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
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
        
        scriptWorld.SetFiles(
            "ScriptExample.so",
            "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/ScriptInclude",
            { "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Scripts/ScriptTest.cpp" },
            defaultIncludes
        );
        
        GameWorld world;
        scriptWorld.SetWorldType(world);
        
        scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
    
        scriptWorld.AddGameWorld(world);
    
        GameObject* root = world.CreateRoot();
        root->AddComponent(0);
        root->AddComponent(1);
        
        GameObject* root2 = world.CreateRoot();
        root2->AddComponent(0);
        root2->AddComponent(1);
        
        auto types = root2->GetComponentTypes([](int i) {
            return true;
        });
        
        auto* fieldType = types[1].GetField("velocity");
        FieldInfo<int>* info = (FieldInfo<int>*)fieldType;
        
        *info->field = 12;
        
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
        
        root2->ToJson(std::cout, 0);
    
        return true;
    });
    
}
