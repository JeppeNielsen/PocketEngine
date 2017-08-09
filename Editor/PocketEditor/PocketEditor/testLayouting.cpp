//
//  testLayouting.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 09/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "Gui.hpp"

using namespace Pocket;

class TestLayouting : public GameState<TestLayouting> {
public:
    GameWorld world;
    GameObject* root;
    Gui* gui;
    GameObject* area;
    GameObject* tabArea;
    
    void Initialize() {
    
        root = world.CreateRoot();
        gui = root->CreateSystem<Gui>();
        
        gui->Setup("images.png", "images.xml", Context().Viewport());
        gui->CreateFont("/Library/Fonts/Arial Bold.ttf");//, "Font");
        
        area = gui->CreateLayoutControl(root, "TextBox", 25, {2000,25}, {2000,25});
        area->GetComponent<Transform>()->Position = {100,100,0};
        tabArea = gui->CreateLayoutControl(area, "TextBox", 25, {500,25}, {500,25}, Layouter::LayoutMode::Horizontal);
        
        CreateTab("Project");
        CreateTab("Tab3");
        CreateTab("Home");
        
        Input.ButtonDown.Bind([this](ButtonEvent b){
            CreateTab("New Tab");
        });
    }
    
    
    void CreateTab(const std::string& name) {
        
        GameObject* button = gui->CreateLayoutControl(tabArea, "Box", 25, {200,25}, {200,25}, Layouter::LayoutMode::Horizontal);
        GameObject* label = gui->CreateLabel(button, 0, {200,30}, 0, name, 12);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
        
        gui->AddLayouter(label, 25, {175,25}, {175,25});
        
        {
            GameObject* closeButton = gui->CreateLayoutControl(button, "Box", 25,25,25);
            
            GameObject* label = gui->CreateLabel(closeButton, 0, {20,20}, 0, "X", 12);
            label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
            label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
            label->GetComponent<Colorable>()->Color = Colour::Black();
            label->ReplaceComponent<Sizeable>(closeButton);
        }
    }
    
    
    void Update(float dt) {
        area->GetComponent<Sizeable>()->Size = Context().InputDevice().GetTouchPosition(0);// Context().ScreenSize;
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }
    
    Colour color;
    
    void Render() {
        //glClearColor(color.R(), color.G(), color.B(), 1);
        //	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_CULL_FACE);
        world.Render();
    }
};

int main_layouting() {
    Engine e;
    e.Start<TestLayouting>();
	return 0;
}
