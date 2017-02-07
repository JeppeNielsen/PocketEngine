#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "File.hpp"
#include "FileArchive.hpp"
#include "OpenWorld.hpp"
#include "AssetManager.hpp"
#include "FontTextureSystem.hpp"

using namespace Pocket;

class RotatingCube : public GameState<RotatingCube> {
public:
    GameWorld world;
    GameObject* camera;
    GameObject* cube;
    GameObject* root;
    FileArchive fileArchive;
    
    void Initialize() {
    
        std::string zipFile = "/Projects/PocketEngine/Editor/resources.zip";
    
        fileArchive.Initialize(zipFile);
        File::SetArchive(fileArchive);
        
        world.GuidToRoot = [this] (const std::string& guid) {
            GameObject* root = 0;
            if (!fileArchive.TryLoadData(guid, [&root, this] (void* data, size_t size) {
                std::stringstream ss;
                ss.write((const char*)data, size);
                root = world.CreateRootFromJson(ss, [](GameObject* root) {
                    OpenWorld::CreateDefaultSystems(*root);
                    root->CreateSystem<AssetManager>();
                    root->CreateSystem<FontTextureSystem>();
                });
            })) {
                std::cout << "unable to load: "<<guid <<std::endl;
            }
            return root;
        };
        world.GuidToPath = [] (const std::string& guid) { return guid; };
        root = world.TryFindRoot("NDGyhUcqSkWLeRYFTKAdqA==");
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }
    
    void Render() {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main_vector() {
    Engine e;
    e.Start<RotatingCube>();
	return 0;
}
