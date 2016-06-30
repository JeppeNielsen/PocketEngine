#include "Engine.hpp"
#include "Gui.hpp"
#include <fstream>
#include "Timer.hpp"
#include <sstream>

using namespace Pocket;

class GuiExample : public GameState<GuiExample> {
public:
    GameWorld world;
    
    GameObject* camera;
    GameObject* cube;
    GameObject* atlas;
    
    struct ClickColorSystem : GameSystem<Mesh, Touchable> {
    
        int number = 5;
    
        void Click(TouchData e, GameObject* go) {
            auto& verts = go->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
            for (int i=0; i<verts.size(); i++) {
                verts[i].Color = Colour::HslToRgb(i * 10 + number*10, 1, 1, 1);
            }
            
            number++;
        }
    
        void ObjectAdded(GameObject* go) {
            go->GetComponent<Touchable>()->Click.Bind(this, &ClickColorSystem::Click, go);
        }
        
        void ObjectRemoved(GameObject* go) {
            go->GetComponent<Touchable>()->Click.Unbind(this, &ClickColorSystem::Click, go);
        }
    };

    
    void Initialize() {
        
        const Box box;
        
        std::stringstream s;
        
        s<<box;
        
        Gui* gui = world.CreateSystem<Gui>();
        world.CreateSystem<ClickColorSystem>();
    
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->CreateFont("Font.fnt", "Font");
     
        auto control = gui->CreateControl(0, "Box", 300);
        control->AddComponent<Draggable>();
        
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main_main() {
    Engine e;
    e.Start<GuiExample>();
	return 0;
}