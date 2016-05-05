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
    selectables = world.CreateSystem<SelectableCollection<EditorObject>>();

    
    scriptWorld.SetFiles(
        "ScriptExample.so",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptInclude",
        {
            "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptCode/ScriptExample.hpp"
        },
        {
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
        }
        );
    
    
}

void CompilationWindow::OnCreate() {
    GameObject* button = CreateButton(window, 10, {100,50}, "Compile");
    button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
       Compile();
    });
}

void CompilationWindow::Compile() {

    GameWorld& world = context->Project().World();

    std::cout<<"Compilation Started..."<<std::endl;


    scriptWorld.RemoveGameWorld(world);
    scriptWorld.SetWorldType(world);
    scriptWorld.Build();
    scriptWorld.AddGameWorld(world);



    GameObject* go = world.CreateObject();

    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    go->AddComponent<Material>();
    go->AddComponent<EditorObject>();
    
    go->AddScriptComponent(1);

    /*
    GameObject* window = gui->CreateControl(0, "Box", {200,200});
    window->AddComponent<Draggable>();
    window->GetComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,0.5f);
    window->GetComponent<Transform>()->Position = {300,300};
    window->AddScriptComponent(1);
    */
    
    std::cout<<"Compilation Ended..." <<std::endl;
}