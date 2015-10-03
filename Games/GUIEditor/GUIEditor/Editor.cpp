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
#include "HierarchyEditorSystem.hpp"

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
    SelectableCollection* selected;
    TextBoxSystem* textboxSystem;
    
    GameObject* editor;
    GameObject* colorBox;
    GameObject* listbox;
    GameObject* listBoxPivot;
    GameObject* root;
    GameObject* label;
    GameObject* box;
    
    void Initialize() {
        
        HierarchyEditorSystem* hierarchyEditorSystem = world.CreateSystem<HierarchyEditorSystem>();
        
        gui = world.CreateFactory<Gui>();
        textboxSystem = world.GetSystem<TextBoxSystem>();
        world.CreateSystem<ClickSelectorSystem>();
        world.CreateSystem<SizeModifierSystem>();
        world.CreateSystem<SizeModifierNodeSystem>();
        world.CreateSystem<SizeModifierLineSystem>();
        selected = world.CreateSystem<SelectableCollection>();
        selected->SelectionChanged += event_handler(this, &Editor::SelectionChanged);
        //world.CreateSystem<GridSystem>();
        GameObjectEditorSystem* gameObjectEditorSystem = world.CreateSystem<GameObjectEditorSystem>();
        gameObjectEditorSystem->gui = gui;
        gameObjectEditorSystem->IgnoreComponent<Atlas>();
        gameObjectEditorSystem->IgnoreComponent<Touchable>();
        
        world.CreateSystem<FieldEditorSystem>()->gui = gui;
        hierarchyEditorSystem->gui = gui;
        
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        gui->CreateFont("Font.fnt", "Font");
        
        root = gui->CreatePivot();
        
        box = gui->CreateControl(root, "Box", 200, 200);
        box->AddComponent<Draggable>();
        box->AddComponent<Selectable>();
        box->AddComponent<SizeModifier>();
        box->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        box->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
        box->AddComponent<Speed>()->speed = {10,23};
        
        
        for (int i=0; i<2; i++) {
    
            GameObject* child = gui->CreateControl(box, "TextBox", {i*20.0f,0}, 50);
            child->AddComponent<Draggable>();
            child->AddComponent<Selectable>();
            child->AddComponent<SizeModifier>();
            child->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
            child->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
            
        
        
            GameObject* subChild = gui->CreateControl(child, "Box", {i*10.0f,0}, 30);
            subChild->AddComponent<Draggable>();
            subChild->AddComponent<Selectable>();
            subChild->AddComponent<SizeModifier>();
            subChild->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
            subChild->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
            
        }
        
        
        label = gui->CreateLabel(root, {200,200}, {200,200}, 0, "Testing label", 20);
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->AddComponent<Touchable>();
        label->AddComponent<Selectable>();
        label->AddComponent<SizeModifier>();
        label->AddComponent<Draggable>();
        
        
        {
            GameObject* window = gui->CreatePivot();
            GameObject* windowBar = gui->CreateControl(window, "TextBox", {0,400},{200,20});
            window->AddComponent<Draggable>();
            window->AddComponent<Touchable>(windowBar);
            
            listbox = gui->CreateListbox(window, "Box", 0, {200,400}, &listBoxPivot);
            listbox->AddComponent<Draggable>();
            
            editor = listBoxPivot;
            editor->AddComponent<GameObjectEditor>()->Object = box;
            
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
        }
        
        {
            GameObject* window = gui->CreatePivot();
            window->GetComponent<Transform>()->Position = {500,200};
            GameObject* windowBar = gui->CreateControl(window, "TextBox", {0,400},{200,20});
            window->AddComponent<Draggable>();
            window->AddComponent<Touchable>(windowBar);
        
            GameObject* listboxPivot;
            GameObject* listbox = gui->CreateListbox(window, "Box", 0, {200,400}, &listboxPivot);
            listbox->AddComponent<Draggable>();
            
            
            GameObject* hierarchyEditor = gui->CreatePivot(listboxPivot);
            hierarchyEditor->AddComponent<Sizeable>()->Size = {200,50};
            hierarchyEditor->AddComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
            hierarchyEditor->GetComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
            hierarchyEditor->AddComponent<HierarchyEditor>()->Object = root;
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
        if (textboxSystem->ActiveTextBox()) return;
        
        if (button == "\x7f") {
            for (auto o : selected->Selected()) {
                o->Remove();
            }
        } else if (button == "p") {
            label->Parent = box;
            label->GetComponent<Transform>()->Position = 0;
        } else if (button == "d") {
            for (auto o : selected->Selected()) {
                o->Clone();
            }
        }
    }
    
    void Update(float dt) {
        world.Update(dt*0.5f);
        world.Update(dt*0.5f);
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