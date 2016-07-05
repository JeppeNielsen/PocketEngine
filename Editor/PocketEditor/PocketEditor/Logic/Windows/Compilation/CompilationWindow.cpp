//
//  CompilationWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "CompilationWindow.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "EditorObject.hpp"

std::string CompilationWindow::Name() { return "Compiler"; }

void CompilationWindow::OnInitialize() {
    GameWorld& world = context->World();
    ScriptWorld& scriptWorld = context->ScriptWorld();
    selectables = world.CreateSystem<SelectableCollection<EditorObject>>();

    scriptWorld.Types.Add<Vector3>();

    scriptWorld.SetClangSdkPath("/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/");
    
    scriptWorld.SetFiles(
        "ScriptExample.so",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptInclude",
        {
            "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptCode/ScriptExample.hpp",
            //"/Projects/PocketEngine/Pocket/Rendering/Colour.cpp"
        }, {
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector2.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
        }
        /*
        {
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Rendering/Mesh.hpp",
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector2.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/VertexMesh.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/TextureAtlas.hpp",
            "/Projects/PocketEngine/Pocket/Rendering/Colour.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Interaction/Touchable.hpp",
        }
        */
        );
    
    
}

void CompilationWindow::OnCreate() {
    GameObject* button = CreateButton(window, 10, {100,50}, "Compile");
    button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
       Compile();
    });
}

void CompilationWindow::Compile() {

    selectables->ClearSelection();

    GameWorld& world = context->Project().World();
    ScriptWorld& scriptWorld = context->ScriptWorld();

    std::cout<<"Compilation Started..."<<std::endl;


    scriptWorld.RemoveGameWorld(world);
    scriptWorld.SetWorldType(world);
    scriptWorld.Build(true);
    scriptWorld.AddGameWorld(world);



    GameObject* go = world.CreateObject();

    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    go->AddComponent<Material>();
    go->AddComponent<Touchable>();
    go->AddComponent<EditorObject>();
    
    //for(auto c : scriptWorld.Components()) {
    //    go->AddComponent(c.second);
    //}
    
    /*
    for(int i=0; i<scriptWorld.ComponentCount(); ++i) {
        go->AddComponent(0);
    }
    */
    
    /*
    TypeInfo info = scriptWorld.GetTypeInfo(*go, scriptWorld.Components()[""]);
    
    {
        
    
    }
    */

    /*
    GameObject* window = gui->CreateControl(0, "Box", {200,200});
    window->AddComponent<Draggable>();
    window->GetComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,0.5f);
    window->GetComponent<Transform>()->Position = {300,300};
    window->AddScriptComponent(1);
    */
    
    std::cout<<"Compilation Ended..." <<std::endl;
}