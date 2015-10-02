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
#include "GameObjectEditorSystem.hpp"

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
    GameObject* editor;
    
    GameObject* colorBox;
    GameObject* listbox;
    GameObject* listBoxPivot;
    
    void Initialize() {
    
               
    
        //SerializedFieldEditor<float> floatT;
        //floatT.Initialize((float*)0,(void*)0);
        
        
        
        gui = world.CreateFactory<Gui>();
        world.CreateSystem<ClickSelectorSystem>();
        world.CreateSystem<SizeModifierSystem>();
        world.CreateSystem<SizeModifierNodeSystem>();
        world.CreateSystem<SizeModifierLineSystem>();
        selected = world.CreateSystem<SelectableCollection>();
        selected->SelectionChanged += event_handler(this, &Editor::SelectionChanged);
        //world.CreateSystem<GridSystem>();
        world.CreateSystem<GameObjectEditorSystem>()->gui = gui;
        world.CreateSystem<FieldEditorSystem>()->gui = gui;
        
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        
        box = gui->CreateControl(0, "Box", 200, 200);
        box->AddComponent<Draggable>();
        box->AddComponent<Selectable>();
        box->AddComponent<SizeModifier>();
        box->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        box->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
        box->AddComponent<Speed>()->speed = {10,23};
        
        GameObject* label = gui->CreateLabel(0, {200,200}, {200,200}, 0, "Testing label", 20);
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->AddComponent<Touchable>();
        label->AddComponent<Selectable>();
        label->AddComponent<SizeModifier>();
        label->AddComponent<Draggable>();
        
        GameObject* window = gui->CreatePivot();
        GameObject* windowBar = gui->CreateControl(window, "TextBox", {0,400},{200,20});
        window->AddComponent<Draggable>();
        window->AddComponent<Touchable>(windowBar);
        
        listbox = gui->CreateListbox(window, "Box", 0, {200,400}, &listBoxPivot);
        listbox->AddComponent<Draggable>();
        
        editor = listBoxPivot;
        editor->AddComponent<GameObjectEditor>()->Object = label;
        
        for (int i=0; i<0; i++) {
            GameObject* child = gui->CreateControl(listBoxPivot, "Box", 0, {300,30});
            child->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
            child->GetComponent<Colorable>()->Color = Colour::HslToRgb(i*18, 1, 1, 1);
            child->GetComponent<Touchable>()->ClickThrough = true;
            GameObject* close = gui->CreateControl(child, "Box", {300-30,0}, {30});
            close->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Right;
            close->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
            close->GetComponent<Touchable>()->Click += event_handler(this, &Editor::CloseClicked, child);
        }
        
        Input.ButtonDown += event_handler(this, &Editor::ButtonDown);
    }
    
    void CloseClicked(TouchData d, GameObject* object) {
        object->Parent = object->Parent() ? 0 : listBoxPivot;
        
    }
    
    void SelectionChanged(SelectableCollection* selectables) {
        if (selectables->Selected().empty()) {
            editor->GetComponent<GameObjectEditor>()->Object = 0;
        } else {
            editor->GetComponent<GameObjectEditor>()->Object = selectables->Selected()[0];
        }
    }
    
    void ButtonDown(std::string button) {
        //editor->GetComponent<FieldEditor>()->Field = "Scale";
    }
    
    void Update(float dt) {
        world.Update(dt*0.5f);
        world.Update(dt*0.5f);
        //box->GetComponent<Speed>()->speed += {0,0.01f};
        //box->GetComponent<Transform>()->Position += {10 * dt,0,0};
        //std::cout<<box->GetComponent<Speed>()->speed<<std::endl;
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