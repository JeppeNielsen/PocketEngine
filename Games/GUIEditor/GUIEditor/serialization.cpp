#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "Colorable.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "File.hpp"

using namespace Pocket;

class TestSerialization : public GameState<TestSerialization> {
public:
    GameWorld world;
    Gui* gui;
    GameObject* font;
    GameObject* box;
    GameObject* label;
    
    void Initialize() {
    
        
        gui = world.CreateFactory<Gui>();
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        
       // box = gui->CreateControl(0, "Box", 100, 200);
       // box->AddComponent<Draggable>();
        
        //label = gui->CreateLabel(box, {0,0}, 200, font, "Testing new Gui Editor, yay \n line break", 11);
        //label->AddComponent<TextBox>()->Text = "Hvad er dit navn?";
        //label->AddComponent<Touchable>(box);
        
        //box->ToJson(std::cout);
        
        //GameObject* child = box->Clone();
        //child->Parent = box;
        
        //std::ofstream saveFile("GameObjectJson.txt");
        //box->ToJson(saveFile);
        //saveFile.close();
        
        
        
        //std::stringstream s;
        //box->ToJson(s);
        //world.CreateObjectFromJson(s);
        
        
        std::ifstream myfile (File::GetFullPath("GameObjectJson.txt"));
        box = world.CreateObjectFromJson(myfile);
        myfile.close();
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main_nono() {
    Engine e;
    e.Start<TestSerialization>();
	return 0;
}