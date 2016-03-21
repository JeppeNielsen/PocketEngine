//
//  Editor.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "Gui.hpp"
#include "ScriptWorld.hpp"

using namespace Pocket;

class Editor : public GameState<Editor> {
public:
    GameWorld world;
    ScriptWorld scriptWorld;
    Gui* gui;
    
    struct Rotator { Vector3 speed; };
    struct RotatorSystem : public GameSystem<Transform, Rotator> {
        void Update(float dt) {
            for(auto o : Objects()) {
                o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->speed * dt;
            }
        }
    };
    
    void Initialize() {
    
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
        
        
        
        gui = world.CreateSystem<Gui>();
        world.CreateSystem<RotatorSystem>();
        scriptWorld.SetWorldType(world);
        
        scriptWorld.Build();
        
        scriptWorld.AddGameWorld(world);
        
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->CreateFont("Font.fnt", "Font");
        
        for (int x=0; x<30; x++) {
            for(int y=0; y<15; y++) {
                CreateCube({(float)x*50,(float)y*50,-0.05f});
            }
        }
        
        GameObject* window = gui->CreateControl(0, "Box", {200,200});
        window->AddComponent<Draggable>();
        window->GetComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,0.5f);
        window->GetComponent<Transform>()->Position = {300,300};
        window->AddScriptComponent(1);
        
        GameObject* compileButton = gui->CreateControl(0, "Box", {100,50});
        gui->CreateLabel(compileButton, 0, {100,50}, 0, "Compile", 20);
        
        compileButton->GetComponent<Touchable>()->Click.Bind([this](auto t) {
            Compile();
        });
    }
    
    void CreateCube(Vector3 pos) {
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = pos;
        cube->AddComponent<Rotator>()->speed = { 0,0,1 };
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, {40,40}, Colour::White());
        cube->AddComponent<Material>();
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 30, 1, 1, 1);
        }
    }
    
    void Compile() {
        std::cout<<"Compilation Started..."<<std::endl;
    
        scriptWorld.RemoveGameWorld(world);
        scriptWorld.Build();
        scriptWorld.AddGameWorld(world);
        
        GameObject* window = gui->CreateControl(0, "Box", {200,200});
        window->AddComponent<Draggable>();
        window->GetComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,0.5f);
        window->GetComponent<Transform>()->Position = {300,300};
        window->AddScriptComponent(1);
        
        std::cout<<"Compilation Ended..." <<std::endl;
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        //glClearColor(1, 1, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}