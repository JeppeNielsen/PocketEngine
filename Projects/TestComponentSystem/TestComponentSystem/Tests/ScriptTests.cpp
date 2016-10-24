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
        
        ScriptWorld scriptWorld;
        scriptWorld.SetClangSdkPath("/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/");
        
        scriptWorld.SetFiles(
            "ScriptExample.so",
            "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptInclude",
            { "/Projects/PocketEngine/Projects/TestComponentSystem/TestComponentSystem/Scripts/ScriptTest.cpp" },
            {}
        );
        
        scriptWorld.Build(true, "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Products/Debug/libPocketEngine.a");
    
        GameWorld world;
        
    
    
        return true;
    });
    
}
