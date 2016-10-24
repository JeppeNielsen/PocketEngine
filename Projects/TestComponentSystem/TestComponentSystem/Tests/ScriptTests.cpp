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
    
        world.Update(1.0f);
    
        return true;
    });
    
}
