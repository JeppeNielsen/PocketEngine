#include "Engine.hpp"
#include "Gui.hpp"
#include <fstream>
#include "Timer.hpp"

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
        
        
        Gui* gui = world.CreateSystem<Gui>();
        world.CreateSystem<ClickColorSystem>();
    
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->CreateFont("Font.fnt", "Font");
        
        
        bool load = false;
        
        if (!load) {
            
            auto window = gui->CreateControl(0, "Box", 100, 200);
            window->AddComponent<Draggable>();
            
            gui->CreateControl(window, "TextBox", 50,100);
            gui->CreateTextBox(window, "Box", 0, {200,30}, 0, "This is a textbox", 20);
            
            Timer timer;
            timer.Begin();
            for(int i=1; i<10; i++)
            {
                auto clone = window->Clone();
                std::stringstream s;
                s<<"Window #"<<i;
                clone->Children()[1]->Children()[0]->GetComponent<TextBox>()->Text = s.str();
            }
            double time = timer.End();
            std::cout << "Create windows : " << time << std::endl;
        
        } else {
            
            auto window = gui->CreateControl(0, "Box", 100, 200);
            window->AddComponent<Draggable>();

            gui->CreateControl(window, "TextBox", 50,100);
            gui->CreateTextBox(window, "Box", 0, {200,30}, 0, "This is a textbox", 20);

            std::stringstream memory;
            window->ToJson(memory);
            
            
            /*
            {
                std::ofstream file;
                file.open("Window.json");
                window->ToJson(file);
                file.close();
            }
            */
            
            
            Timer timer;
            timer.Begin();
            for(int i=0; i< 200; i++)
            {
                memory.seekg(0, std::ios::beg);
                //std::ifstream file;
                //file.open("Window.json");
                world.CreateObject(memory);
                //file.close();
            }
            double time = timer.End();
            std::cout << "Load windows : " << time << std::endl;
        }
        
        
        /*
        
        atlas = world.CreateObject();
        atlas->AddComponent<Atlas>()->Load("images.xml", {1024, 512});
        atlas->AddComponent<TextureComponent>()->Texture().LoadFromPng("images.png");
        
        
        GameObject* quad = world.CreateObject();
        quad->AddComponent<Transform>()->Position = {0,0,-10};
        quad->AddComponent<Sprite>()->SpriteName = "Box";
        quad->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        quad->AddComponent<Mesh>();
        quad->AddComponent<Atlas>(atlas);
        quad->AddComponent<TextureComponent>(atlas);
        quad->AddComponent<Sizeable>()->Size = {100,100};
        quad->AddComponent<Draggable>();
        quad->AddComponent<Touchable>();
        
        GameObject* label = world.CreateObject();
        label->Parent = quad;
        label->AddComponent<Transform>();
        label->AddComponent<Font>()->Load("Font.fnt", "Font");
        label->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        label->AddComponent<Mesh>();
        label->AddComponent<Atlas>(atlas);
        label->AddComponent<TextureComponent>(atlas);
        label->AddComponent<Sizeable>()->Size = {100,100};
        label->AddComponent<Label>()->Text = "Jeppe er den bedste";
        label->GetComponent<Label>()->FontSize = 10;
        
        auto& renderSystem = world.GetSystem<RenderSystem<GameWorldSettings>>();
        
        world.GetSystem<TouchSystem<GameWorldSettings>>().Input = &Input;
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 0, 470 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        */
        
        //CreateObject(0);
    }
    /*
    GameObject* CreateObject(Vector3 pos) {
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = pos;
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({-2,0,0}, {20,10,10});
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::Blue();// Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        cube->AddComponent<Material>();
        cube->AddComponent<Touchable>()->Click.Bind([cube] (TouchData d) {
            cube->GetComponent<Transform>()->Rotation *= Quaternion(0.1f);
        });
        return cube;
    }
    */
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<GuiExample>();
	return 0;
}