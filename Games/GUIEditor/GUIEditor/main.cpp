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
#include "GridSystem.h"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    Gui* gui;
    GameObject* font;
    GameObject* box;
    
    void Initialize() {
        
        gui = world.CreateFactory<Gui>();
        world.CreateSystem<ClickSelectorSystem>();
        world.CreateSystem<SizeModifierSystem>();
        world.CreateSystem<SizeModifierNodeSystem>();
        world.CreateSystem<SizeModifierLineSystem>();
        
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        world.CreateSystem<GridSystem>();
        
        
        
        box = gui->CreateControl(0, "Box", 100, 200);
        box->AddComponent<Draggable>();
        box->AddComponent<Selectable>();
        box->AddComponent<SizeModifier>();
        box->AddComponent<Gridable>()->Size = 10;
        box->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        box->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
        
        
        GameObject* label = gui->CreateLabel(box, 0, 200, font, "Testing label control", 12);
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        label->GetComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
        label->GetComponent<Label>()->WordWrap = true;
        label->GetComponent<Label>()->HAlignment = Font::Center;
        label->GetComponent<Label>()->VAlignment = Font::Middle;
        
        label->AddComponent<Selectable>();
        label->AddComponent<Touchable>();
        label->AddComponent<SizeModifier>();
        label->AddComponent<Draggable>();
        
        label->Clone();
        
        box->Clone()->Parent = box;
        
        
        /*
        auto clone = box->Clone();
        clone->Parent = box;
        box->GetComponent<Transform>()->Rotation = 0.5f;
        box->GetComponent<Transform>()->Anchor = {100,100,0};
        */
        
        //std::ifstream file(File::GetFullPath("GameObjectJson.txt"));
        //box = world.CreateObjectFromJson(file);
        //file.close();
        
        /*
        GameObject* bla = world.CreateObject();
        bla->AddComponent<Transform>()->Position = {500,500,0};
        bla->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 300, Colour::White());
        bla->AddComponent<TextureComponent>(gui->Atlas());
        bla->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        */
        //GameObject* label = gui->CreateLabel(box, {0,0}, 200, font, "Testing new Gui Editor, yay \n line break", 11);
        //label->AddComponent<TextBox>()->Text = "Hvad er dit navn?";
        //label->AddComponent<Touchable>(box);
        //label->GetComponent<Colorable>()->Color = Colour::Black();
        Input.ButtonDown += event_handler(this, &Game::ButtonDown);
        
       // box->GetComponent<Transform>()->Rotation = 0.2f;
    }
    
    void ButtonDown(std::string button) {
    
        if (button == "s") {
            std::ofstream file("Filename.txt");
            box->ToJson(file);
            file.close();
        } else if (button == "l") {
            std::ifstream file(File::GetFullPath("GameObjectJson.txt"));
            box = world.CreateObjectFromJson(file);
            file.close();
        } else if (box && button == "d") {
            //box->Remove();
            //box = 0;
        }
    }
    
    
    void Update(float dt) {
    static float rot = 0;
        rot+=dt*0.1f;
        
        world.Update(dt*0.5f);
        world.Update(dt*0.5f);
      
        
    }
    
    void Render() {
        glClearColor(0, 0, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
	return 0;
}