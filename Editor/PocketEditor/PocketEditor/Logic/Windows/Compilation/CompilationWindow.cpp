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
#include "Timer.hpp"

std::string CompilationWindow::Name() { return "Compiler"; }

void CompilationWindow::OnInitialize() {
    GameWorld& world = context->World();
    selectables = world.CreateSystem<SelectableCollection<EditorObject>>();
}

void CompilationWindow::OnCreate() {
    {
        GameObject* button = CreateButton(window, 10, {100,50}, "Compile");
        button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
           Compile();
        });
    }
    
    {
        GameObject* button = CreateButton(window, {10,60}, {100,50}, "Build");
        button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
           Build();
        });
    }
}

void CompilationWindow::Compile() {

    
    
    selectables->ClearSelection();

    
    Pocket::Timer timer;
    timer.Begin();
    std::cout<<"Compilation Started..."<<std::endl;
    context->Project().Compile();
    std::cout<<"Compilation Ended... time = "<< timer.End() <<" seconds "<<std::endl;
//
//        scriptWorld.RemoveGameWorld(world);
//        scriptWorld.SetWorldType(world);
//        scriptWorld.Build(true);
//        scriptWorld.AddGameWorld(world);


    GameWorld& world = context->Project().World();

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
    
    
}

void CompilationWindow::Build() {
    std::cout<<"Build Started..."<<std::endl;
    Pocket::Timer timer;
    timer.Begin();
    context->Project().Build();
    std::cout<<"Build Ended... time = "<<timer.End()<<"seconds" <<std::endl;}