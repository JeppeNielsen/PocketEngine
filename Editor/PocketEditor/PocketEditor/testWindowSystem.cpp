//
//  testWindowSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "Gui.hpp"

using namespace Pocket;

class TestWindowSystem : public GameState<TestWindowSystem> {
public:
    GameWorld world;
    GameObject* root;
    Gui* gui;
    GameObject* area;
    
    GameObject* CreateArea(Vector2 size) {
        GameObject* object = gui->CreateControl(root, "", 0, size);
        object->AddComponent<PanelArea>();
        object->RemoveComponent<Renderable>();
        return object;
    }
    
    Panel* CreatePanel(Vector2 pos, Vector2 size, std::string text) {
        GameObject* window = gui->CreateControl(root, "TextBox", pos, size);
        window->AddComponent<Draggable>();
        window->AddComponent<Panel>();
        window->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
        
        GameObject* content = gui->CreateLayout(window, 20, {2000,2000}, {2000,2000}, Layouter::LayoutMode::Vertical);
        
        GameObject* horizontal = gui->CreateLayout(content, 20, {2000,20}, {2000,20}, Layouter::LayoutMode::Horizontal);
        {
            gui->CreateLayout(horizontal, 20, 20, {50,20});
            GameObject* button = gui->CreateTextBox(horizontal, "Box", 0, 0, 0, "Text", 20);//(horizontal, "Box", 30, {200,30}, {2000,30});
            gui->AddLayouter(button, 30, {200,30}, {2000,30});
            gui->CreateLayout(horizontal, 20, 20, {50,20});
        }
        GameObject* fill = gui->CreateLayout(content, 20, {2000,2000}, {2000,2000});
        
        
        GameObject* bar = gui->CreateLayoutControl(window, "Box", 20, {2000,20}, {2000,20});
        bar->AddComponent<Droppable>()->OnCreate = [this](GameObject* o, TouchData d) -> GameObject* {
            Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
            GameObject* control = gui->CreateControl(0, "Box", position, {200,25});
            return control;
        };
        bar->AddComponent<PanelDropper>()->panel = window;
        auto label = gui->CreateLabel(bar, 0, 0, 0, text, 16);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->ReplaceComponent<Sizeable>(bar);
        
        return window->GetComponent<Panel>();
    }
    
    void Initialize() {
    
        root = world.CreateRoot();
        gui = root->CreateSystem<Gui>();
        
        gui->Setup("images.png", "images.xml", Context().Viewport());
        gui->CreateFont("/Library/Fonts/Arial Bold.ttf");//, "Font");
        
        area = CreateArea(Context().Viewport().Size());
        
        auto p = CreatePanel({0,50},{200,500}, "Project");
        //p->Area = area;
        
        CreatePanel({250,50},{200,500}, "Systems");
        CreatePanel({500,50},{200,500}, "Inspector");
        CreatePanel({560,50},{200,300}, "Extra");
        CreatePanel({600,50},{200,300}, "Animation");
        
        //Input.ButtonDown.Bind([this](ButtonEvent b){
        //    color = Colour::HslToRgb(MathHelper::Random(0, 360), 1, 1, 1);
        //});
    }
    
    void Update(float dt) {
        area->GetComponent<Sizeable>()->Size = Context().ScreenSize;
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

int main() {
    Engine e;
    e.Start<TestWindowSystem>();
	return 0;
}
