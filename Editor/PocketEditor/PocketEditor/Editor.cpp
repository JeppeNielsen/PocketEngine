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
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"

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
    
    struct TouchInvalidSystem : public GameSystem<Colorable, Touchable> {
    
        void Update(float dt) {
            for(auto o : Objects()) {
                o->GetComponent<Colorable>()->Color = o->GetComponent<Touchable>()->Click.Empty() ? Colour::Red() : Colour::Green();
            }
        
        }
    
    };
    
    void Initialize() {
        
        /*
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
        */
        
        
        
        gui = world.CreateSystem<Gui>();
        world.CreateSystem<RotatorSystem>();
        
        world.CreateSystem<VirtualTreeListSystem>();
        world.CreateSystem<VirtualTreeListSpawnerSystem>();
        //world.CreateSystem<TouchInvalidSystem>();
        
        /*
        scriptWorld.SetWorldType(world);
        
        scriptWorld.Build();
        
        scriptWorld.AddGameWorld(world);
        */
        
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->CreateFont("Font.fnt", "Font");
        
        
        GameObject* pivot;
        GameObject* listBox = gui->CreateListbox(0, "Box", {0,0}, {200,800}, &pivot);
        
        
        GameObject* root = world.CreateObject();
        //root->ID = "Root";
        
        /*
        GameObject* child1 = world.CreateObject();
        child1->ID = "Child0";
        
        GameObject* child2 = world.CreateObject();
        child2->ID = "Child1";
        
        child1->Parent = root;
        child2->Parent = root;
        */
        
        
        /*
        for (int i=0; i<5; i++) {
            GameObject* child = world.CreateObject();
            std::stringstream s;
            s<<"Root/Child"<<i;
            std::string str =s.str();
            child->ID = str;
            child->Parent = root;
            
            GameObject* sub = world.CreateObject();
            std::stringstream ss;
            ss<<"Root/Child"<<i<<"/Sub";
            sub->ID = ss.str();
            sub->Parent = child;
            
        }
        
        */
        
        
        for(int i=0; i<100; i++) {
            GameObject* child1 = world.CreateObject();
            child1->Parent = root;
            //child1->ID = "Child1";
        
            for(int j=0; j<50; j++) {
            GameObject* child2 = world.CreateObject();
            child2->Parent = child1;
            //child2->ID = "Child2";
        
            GameObject* child3 = world.CreateObject();
            child3->Parent = child2;
            //child3->ID = "Child3";
        
                GameObject* sub1 = world.CreateObject();
                sub1->Parent = child3;
                //sub1->ID = "Sub1";
                GameObject* sub2 = world.CreateObject();
                sub2->Parent = child3;
                //sub2->ID = "Sub2";
            GameObject* child4 = world.CreateObject();
            child4->Parent = child1;
            //child4->ID = "Child4";
            
            }
        }
        
        auto treeView = pivot->AddComponent<VirtualTreeList>();
        treeView->Root = root;
        treeView->SetNodeExpanded(root, true);
        //treeView->SetNodeExpanded(child3, true);
        treeView->Pivot = listBox;
        auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
        

        
        spawner->OnCreate = [&](GameObject* node, GameObject* parent) -> GameObject* {
            GameObject* clone = gui->CreateControl(parent, "Box", {-1000,0}, {200,25});
            clone->RemoveComponent<Touchable>();
            gui->CreateControl(clone, "TextBox", 0, {25,25});
            gui->CreateTextBox(clone, "TextBox", {25,0}, {200-25,25}, 0, "Object", 10);
            return clone;
        };
        
        
        for(int i=0; i<0; i++) {
            GameObject* box = gui->CreateControl(pivot, "Box", {0,i*52.0f}, {200,50});
            box->GetComponent<Colorable>()->Color = Colour::HslToRgb(i*20, 1, 1, 1);
            box->RemoveComponent<Touchable>();
        }
        
        for (int x=0; x<30; x++) {
            for(int y=0; y<15; y++) {
                CreateCube({(float)x*50,(float)y*50,-0.05f});
            }
        }
        
        GameObject* window = gui->CreateControl(0, "Box", {200,200});
        window->AddComponent<Draggable>();
        window->GetComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,0.5f);
        window->GetComponent<Transform>()->Position = {300,300};
       // window->AddScriptComponent(1);
        
        /*
        GameObject* compileButton = gui->CreateControl(0, "Box", {100,50});
        gui->CreateLabel(compileButton, 0, {100,50}, 0, "Compile", 20);
        
        compileButton->GetComponent<Touchable>()->Click.Bind([this](auto t) {
            Compile();
        });
        */
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