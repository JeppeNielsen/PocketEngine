#include "Engine.hpp"
#include "File.hpp"
#include "FileArchive.hpp"

#include "RenderSystem.hpp"
#include "TransformHierarchy.hpp"
#include "TouchSystem.hpp"
#include "ClonerSystem.hpp"
#include "InputMapperSystem.hpp"
#include "VelocitySystem.hpp"
#include "Gui.hpp"
#include "SwitchSystem.hpp"
#include "SwitchEnablerSystem.hpp"
#include "TouchSwitchSystem.hpp"
#include "SlicedQuadMeshSystem.hpp"
#include "AssetManager.hpp"
#include "SystemHelper.hpp"

using namespace Pocket;

void CreateDefaultSystems(Pocket::GameObject &world) {
    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    world.CreateSystem<ClonerSystem>();
    world.CreateSystem<InputMapperSystem>();
    world.CreateSystem<VelocitySystem>();
    world.CreateSystem<Gui>();
    world.CreateSystem<SwitchSystem>();
    world.CreateSystem<SwitchEnablerSystem>();
    world.CreateSystem<TouchSwitchSystem>();
    world.CreateSystem<SlicedQuadMeshSystem>();
    world.CreateSystem<AssetManager>();
}

class GameCode : public GameState<GameCode> {
public:
    GameWorld world;
    FileArchive fileArchive;
    
    void Initialize() {
    
        std::string zipFile = "/Projects/PocketEngine/EditorProjects/Builds2/resources";
    
        fileArchive.Initialize(zipFile);
        File::SetArchive(fileArchive);
        
        world.GuidToRoot = [this] (const std::string& guid) {
            GameObject* root = 0;
            if (!fileArchive.TryLoadData(guid, [&root, guid, this] (void* data, size_t size) {
                std::stringstream ss;
                ss.write((const char*)data, size);
                root = world.CreateRootFromJson(ss, [](GameObject* root) {
                    SystemHelper::AddGameSystems(*root);
                });
            })) {
                std::cout << "unable to load: "<<guid <<std::endl;
            }
            return root;
        };
        world.GuidToPath = [] (const std::string& guid) { return guid; };
        world.SetLayerScene(0, world.TryFindRoot("9KkPi0bmTS6f+W3nOFgVRw=="));
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main_loadgui() {
    Engine e;
    e.Start<GameCode>();
	return 0;
}
