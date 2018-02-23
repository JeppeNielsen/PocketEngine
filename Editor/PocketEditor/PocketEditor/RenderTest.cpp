//
//  RenderTest.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/02/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

//
//  Editor.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"

using namespace Pocket;

class RenderTest : public GameState<RenderTest> {
public:
    GameStorage storage;
    GameWorld world1;
    GameWorld world2;
    
    void Initialize() {
        storage.AddSystemType<Gui>();
        
        world1.Initialize(storage);
        world2.Initialize(storage);
        
        {
            GameObject* scene = world1.CreateScene();
            auto gui = scene->CreateSystem<Gui>();
            gui->Setup("NewUI.tga", "NewUI.json", Context().Viewport());
            gui->CreateFont("SanFranciscoText-Bold.otf");//, "Font");
            
            GameObject* control = gui->CreateControl(scene, "Box", 0, 300);
            control->AddComponent<Draggable>();
            
            gui->CreateLabel(control, 100, 100, nullptr, "Testing ui", 20);
        }
        
        {
            GameObject* scene = world2.CreateScene();
            auto gui = scene->CreateSystem<Gui>();
            gui->Setup("NewUI.tga", "NewUI.json", Context().Viewport());
            gui->CreateFont("SanFranciscoText-Bold.otf");//, "Font");
            
            gui->CreateControl(scene, "Box", {600,0}, 300);
        }
        
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world1.Input());
        Context().InputDevice().UpdateInputManager(&world2.Input());
        world1.Update(dt);
        world2.Update(dt);
    }
    
    void Render() {
        world1.Render();
        world2.Render();
    }
};

int main_rendertest() {
    Engine e;
    e.Start<RenderTest>();
    return 0;
}

