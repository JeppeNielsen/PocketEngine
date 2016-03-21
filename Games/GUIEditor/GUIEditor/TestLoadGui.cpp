//
//  TestLoadGui.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 04/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include <fstream>
#include <functional>

using namespace Pocket;

class TestLoadGui : public GameState<TestLoadGui> {
    GameWorld world;
    GameObject* root;
    
    void Initialize() {
        Gui* gui = world.CreateSystem<Gui>();
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->CreateFont("Font.fnt", "Font");
        std::ifstream file("Gui.txt");
        root = world.CreateObject(file);
        file.close();
        root->GetID();
    }
    
    void Update(float dt) {
        world.Update(dt);
        //root->Children()[0]->GetComponent<Sizeable>()->Size = Input.GetTouchPosition(0)+Vector2(200,200);
    }
    
    void Render() {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main_load() {
    Engine e;
    e.Start<TestLoadGui>();
    return 0;
}