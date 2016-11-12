#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    
    void Initialize() {
    
        GameObject* root = world.CreateRoot();
        Gui* gui = root->CreateSystem<Gui>();
        
        gui->Setup("images.png", "images.xml", Context().Viewport());
        
        auto box = gui->CreateControl(root, "Box", 100, 300);
        box->AddComponent<Draggable>();
        
        auto font = gui->CreateFont("Font.fnt", "Font");
        
        gui->CreateClipper(box, true);
        auto label = gui->CreateLabel(box, 100, 200, font, "Hej med dig", 50);
        gui->CreateClipper(box, false);
        
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main_gui() {
    Engine e;
    e.Start<Game>();
	return 0;
}