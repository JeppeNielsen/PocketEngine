//
//  main_dynamicFont.cpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FontTextureSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "DynamicLabelSystem.hpp"

using namespace Pocket;


struct DynamicFont : public GameState<DynamicFont> {
    
    GameWorld world;
    GameObject* root;

    void Initialize() {
        root = world.CreateRoot();
        root->CreateSystem<RenderSystem>();
        root->CreateSystem<FontTextureSystem>();
        root->CreateSystem<FirstPersonMoverSystem>();
        root->CreateSystem<DynamicLabelSystem>();
        
        GameObject* camera = root->CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = {0,0,5};
        camera->AddComponent<FirstPersonMover>();
        
        GameObject* cube = root->CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Renderable>();
        //cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        
        bool succes = cube->AddComponent<Font>()->LoadTTF("/Users/Jeppe/Library/Fonts/NuevaStd-Bold_ttf.ttf");
        if (!succes) {
            std::cout << "Font could not be loaded"<<std::endl;
        }
        cube->AddComponent<TextureComponent>();
        
        GameObject* label = root->CreateObject();
        label->AddComponent<Transform>();
        label->AddComponent<Sizeable>()->Size = {2,1};
        label->AddComponent<Renderable>();
        label->AddComponent<TextureComponent>(cube);
        label->AddComponent<Mesh>();
        label->AddComponent<Font>(cube);
        label->AddComponent<DynamicLabel>()->Text = "Jeppe er god";
        
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
    e.Start<DynamicFont>();
    return 0;
}
