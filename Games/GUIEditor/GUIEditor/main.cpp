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

class Game : public GameState<Game> {
public:
    GameWorld world;
    Gui* gui;
    GameObject* font;
    
    void Initialize() {
        
        gui = world.CreateFactory<Gui>();
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        
        GameObject* box = gui->CreateControl(0, "Box", 100, 200);
        box->AddComponent<Draggable>();
        
        GameObject* label = gui->CreateLabel(box, {0,0}, 200, font, "Testing new Gui Editor, yay \n line break", 11);
        label->AddComponent<TextBox>()->Text = "Hvad er dit navn?";
        label->AddComponent<Touchable>(box);
        label->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
	return 0;
}