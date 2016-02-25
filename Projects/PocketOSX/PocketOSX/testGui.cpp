#include "Engine.hpp"
#include "Gui.hpp"

struct GameWorldSettings :
    GameSettings<
        Gui<GameWorldSettings>
    >
{};


using namespace Pocket;

class GuiExample : public GameState<GuiExample> {
public:
    GameWorld<GameWorldSettings> world;
    
    using GameObject = GameObject<GameWorldSettings>;
    
    GameObject* camera;
    GameObject* cube;
    GameObject* atlas;
    
    void Initialize() {
        
        world.Initialize();
        
        Gui<GameWorldSettings>& gui = world.GetSystem<Gui<GameWorldSettings>>();
    
        gui.Setup("images.png", "images.xml", Manager().Viewport(), Input);
        
        gui.CreateControl(0, "Box", 100, 200);
         
        
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
        
        CreateObject(0);
    }
    
    GameObject* CreateObject(Vector3 pos) {
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = pos;
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({-2,0,0}, {2,1,1});
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