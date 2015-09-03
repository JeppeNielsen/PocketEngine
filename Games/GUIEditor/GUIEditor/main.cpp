#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "Colorable.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
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
        
        box = gui->CreateControl(0, "Box", 100, 200);
        box->AddComponent<Draggable>();
        
        label = gui->CreateLabel(box, {0,0}, 200, font, "Testing new Gui Editor, yay \n line break", 11);
        
        label->AddComponent<TextBox>()->Text = "Hvad er dit navn?";
        label->AddComponent<Touchable>(box);
        
        
        GameObject* parent = world.CreateObject();
        parent->AddComponent<Transform>()->Position = {1,2,3};
        
        GameObject* child = world.CreateObject();
        child->Parent = parent;
        child->AddComponent<Transform>()->Position = {10,20,30};
        
        
        ISerializedProperty saveData;
        
        parent->Serialize(&saveData);
        
        
        
        
        
    }
    
    void Update(float dt) {
       
        world.Update(dt);
        label->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour::Black());
       
        //std::cout<< box->GetComponent<Orderable>()->Order << " : " << label->GetComponent<Orderable>()->Order<<std::endl;
       
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