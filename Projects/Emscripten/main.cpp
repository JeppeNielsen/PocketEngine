#include "Engine.hpp"
#include "GameSystem.hpp"
#include "File.hpp"
#include "FileArchive.hpp"
#include <iostream>
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

using namespace Pocket;

void CreateDefaultSystems(Pocket::GameObject &world) {
    std::cout << "CreateDefaultSystems 1"<<std::endl;
    world.CreateSystem<RenderSystem>();
    std::cout << "CreateDefaultSystems 2"<<std::endl;
    world.CreateSystem<TransformHierarchy>();
    std::cout << "CreateDefaultSystems 3"<<std::endl;
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    std::cout << "CreateDefaultSystems 4"<<std::endl;
    world.CreateSystem<ClonerSystem>();
    std::cout << "CreateDefaultSystems 5"<<std::endl;
    world.CreateSystem<InputMapperSystem>();
    std::cout << "CreateDefaultSystems 6"<<std::endl;
    world.CreateSystem<VelocitySystem>();
    std::cout << "CreateDefaultSystems 7"<<std::endl;
    world.CreateSystem<Gui>();
    std::cout << "CreateDefaultSystems 8"<<std::endl;
    world.CreateSystem<SwitchSystem>();
    std::cout << "CreateDefaultSystems 9"<<std::endl;
    world.CreateSystem<SwitchEnablerSystem>();
    std::cout << "CreateDefaultSystems 10"<<std::endl;
    world.CreateSystem<TouchSwitchSystem>();
    std::cout << "CreateDefaultSystems 11"<<std::endl;
    world.CreateSystem<SlicedQuadMeshSystem>();
    std::cout << "CreateDefaultSystems 12"<<std::endl;
    world.CreateSystem<AssetManager>();
    std::cout << "CreateDefaultSystems 13"<<std::endl;
}

class GameCode : public GameState<GameCode> {
public:
    GameWorld world;
    FileArchive fileArchive;
    
    void Initialize() {
      
        std::cout << "Initialize 1"<<std::endl;

        std::string zipFile = "resources";
    
        fileArchive.Initialize(zipFile);

        std::cout << "Initialize 2"<<std::endl;

        File::SetArchive(fileArchive);

        std::cout << "Initialize 3"<<std::endl;
        
        world.GuidToRoot = [this] (const std::string& guid) {

        std::cout << "world.GuidToRoot  1"<<std::endl;

            GameObject* root = 0;
            if (!fileArchive.TryLoadData(guid, [&root, this] (void* data, size_t size) {
                std::cout << "world.GuidToRoot 2"<<std::endl;
                std::stringstream ss;
                ss.write((const char*)data, size);
                std::cout << "world.GuidToRoot 3"<<std::endl;

                //std::cout << ss.str() << std::endl;
                try {
                    root = world.CreateRootFromJson(ss, [](GameObject* root) {
                        std::cout << "world.GuidToRoot 4"<<std::endl;
                        CreateDefaultSystems(*root);
                    });
                }
                catch(std::exception e) {
                    std::cout << e.what()<<std::endl;
                }
            })) {
                std::cout << "unable to load: "<<guid <<std::endl;
            }
            std::cout << "world.GuidToRoot  3"<<std::endl;
            return root;
        };


        std::cout << "Initialize 4"<<std::endl;
        world.GuidToPath = [] (const std::string& guid) { return guid; };
        
        try {
            world.TryFindRoot("NDGyhUcqSkWLeRYFTKAdqA==");
        } catch (std::exception e) {
            std::cout << e.what() <<std::endl;
        }
        
        std::cout << "Initialize 5"<<std::endl;

        world.Input().TouchDown.Bind([] (TouchEvent e) {
            std::cout << " Touch down"<< std::endl;
        });
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<GameCode>();
    return 0;
}