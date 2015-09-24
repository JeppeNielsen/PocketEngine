#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "Colorable.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "File.hpp"
#include "SizeModifierSystem.hpp"
#include "SizeModifierNodeSystem.hpp"
#include "SizeModifierLineSystem.h"
#include "ClickSelectorSystem.hpp"
#include "SelectableCollection.hpp"
#include "GridSystem.h"
#include <vector>
#include "FieldEditorSystem.hpp"

using namespace Pocket;

Component(Speed)
    public:
        Speed() : speed(this) { }
        Property<Speed*, Vector2> speed;

    SERIALIZE_FIELDS_BEGIN
    SERIALIZE_FIELD(speed)
    SERIALIZE_FIELDS_END
};

class Editor : public GameState<Editor> {
public:
    GameWorld world;
    Gui* gui;
    GameObject* font;
    GameObject* box;
    SelectableCollection* selected;
    Transform* transform;
    
    void Initialize() {
    
               
    
        //SerializedFieldEditor<float> floatT;
        //floatT.Initialize((float*)0,(void*)0);
        
        
        
        gui = world.CreateFactory<Gui>();
        world.CreateSystem<ClickSelectorSystem>();
        world.CreateSystem<SizeModifierSystem>();
        world.CreateSystem<SizeModifierNodeSystem>();
        world.CreateSystem<SizeModifierLineSystem>();
        selected = world.CreateSystem<SelectableCollection>();
        world.CreateSystem<GridSystem>();
        world.CreateSystem<FieldEditorSystem>()->gui = gui;
        
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        
        box = gui->CreateControl(0, "Box", 100, 200);
        box->AddComponent<Draggable>();
        box->AddComponent<Selectable>();
        box->AddComponent<SizeModifier>();
        box->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        box->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
        box->AddComponent<Speed>()->speed = {10,23};
        
        //GameObject* textBox = gui->CreateTextBox(0, "Box", {500,100}, {200,50}, 0, "Testing Editor", 10);
        
        {
        GameObject* editor = gui->CreateControl(0, "Box", 300, {300,50});
        auto e = editor->AddComponent<FieldEditor>();
        e->Object = box->GetComponent<Transform>();
        e->Field = "Position";
        }
        {
        GameObject* editor = gui->CreateControl(0, "Box", {300,350}, {300,50});
        auto e = editor->AddComponent<FieldEditor>();
        e->Object = box->GetComponent<Sizeable>();
        e->Field = "Size";
        }
        
        Input.ButtonDown += event_handler(this, &Editor::ButtonDown);
    }
    
    void ButtonDown(std::string button) {
    
    }
    
    void Update(float dt) {
        world.Update(dt*0.5f);
        world.Update(dt*0.5f);
        box->GetComponent<Speed>()->speed += {0,0.01f};
        box->GetComponent<Transform>()->Position += {10 * dt,0,0};
        std::cout<<box->GetComponent<Speed>()->speed<<std::endl;
    }
    
    void Render() {
        glClearColor(0, 0.5, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}