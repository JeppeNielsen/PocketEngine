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
#include "StringHelper.hpp"

using namespace Pocket;


struct DynamicFont : public GameState<DynamicFont> {
    
    GameWorld world;
    GameObject* root;
    GameObject* label;
    GameObject* label2;

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
        
        GameObject* font = root->CreateObject();
        bool succes = font->AddComponent<Font>()->LoadTTF("/Users/Jeppe/Library/Fonts/NuevaStd-Bold_ttf.ttf");
        if (!succes) {
            std::cout << "Font could not be loaded"<<std::endl;
        }
        font->AddComponent<TextureComponent>();
        font->AddComponent<Renderable>();
        font->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 40, Colour::White());
        font->AddComponent<Transform>()->Position = {0,0,1};
        
        {
            label = root->CreateObject();
            label->AddComponent<Transform>();
            label->AddComponent<Sizeable>()->Size = {0,0};
            label->AddComponent<Renderable>();
            label->AddComponent<TextureComponent>(font);
            label->AddComponent<Mesh>();
            label->AddComponent<Font>(font);
            label->AddComponent<DynamicLabel>()->Text = "ABCDEFG";
            label->GetComponent<DynamicLabel>()->FontSize = 12;
        }
        
        {
            label2 = root->CreateObject();
            label2->AddComponent<Transform>()->Position = {0,12};
            label2->AddComponent<Sizeable>()->Size = {0,0};
            label2->AddComponent<Renderable>();
            label2->AddComponent<TextureComponent>(font);
            label2->AddComponent<Mesh>();
            label2->AddComponent<Font>(font);
            label2->AddComponent<DynamicLabel>()->Text = "ABCDEFG";
            label2->GetComponent<DynamicLabel>()->FontSize = 64;
        }

        
        {
        GameObject* cube = root->CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {0.02f,6.0f,0.02f});
        cube->AddComponent<Renderable>();
        }
        {
        GameObject* cube = root->CreateObject();
        cube->AddComponent<Transform>()->Position = {1,0,0};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {0.02f,6.0f,0.02f});
        cube->AddComponent<Renderable>();
        }
        counter = 0;
        timer = 2.0f;
    }

    int counter;
float timer;
    void Update(float dt) {
        
        if (timer<0) {
        
        std::stringstream s;
        s<<counter++;
        //label->GetComponent<DynamicLabel>()->Text = s.str();
        timer = 2.0f;
        } else {
            timer -= dt;
        }
        
        label->GetComponent<DynamicLabel>()->FontSize = Input.GetTouchPosition(0).x * 0.1f;
        
        
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
