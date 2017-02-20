//
//  testEditorLoad.cpp
//  PocketIOS
//
//  Created by Jeppe Nielsen on 16/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

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
    GameObject* root;
    
    void Initialize() {
      
        std::string zipFile = "resources";
    
        fileArchive.Initialize(zipFile);

        File::SetArchive(fileArchive);
        
        world.GuidToRoot = [this] (const std::string& guid) {
            GameObject* root = 0;
            if (!fileArchive.TryLoadData(guid, [&root, this] (void* data, size_t size) {
                std::stringstream ss;
                ss.write((const char*)data, size);
                    root = world.CreateRootFromJson(ss, [](GameObject* root) {
                        CreateDefaultSystems(*root);
                    });
            })) {
                std::cout << "unable to load: "<<guid <<std::endl;
            }
            return root;
        };

        world.GuidToPath = [] (const std::string& guid) { return guid; };
        root = world.TryFindRoot("NDGyhUcqSkWLeRYFTKAdqA==");
        
        GameObject* camObject = root->FindObject(12);
        camObject->GetComponent<Camera>()->FieldOfView = 50;
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
