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
#include "LabelMeshSystem.hpp"
#include "StringHelper.hpp"
#include "ColorSystem.hpp"

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
        root->CreateSystem<LabelMeshSystem>();
        root->CreateSystem<ColorSystem>();
        
        GameObject* camera = root->CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = {0,0,5};
        camera->AddComponent<FirstPersonMover>();
        
        GameObject* font = root->CreateObject();
        bool succes = font->AddComponent<Font>()->LoadTTF("/Library/Fonts/Times New Roman.ttf");//"/Users/Jeppe/Library/Fonts/NuevaStd-Bold_ttf.ttf");
        if (!succes) {
            std::cout << "Font could not be loaded"<<std::endl;
        }
        //font->GetComponent<Font>()->CharacterSetEverySize = 2;
        font->AddComponent<TextureComponent>();
        font->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        font->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 40, Colour::White());
        //font->AddComponent<Transform>()->Position = {0,0,1};
        
        {
            label = root->CreateObject();
            label->AddComponent<Transform>();
            label->AddComponent<Sizeable>()->Size = {64,64};
            label->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
            label->AddComponent<TextureComponent>(font);
            label->AddComponent<Mesh>();
            label->AddComponent<Font>(font);
            label->AddComponent<Label>()->Text = "Testing";
            label->GetComponent<Label>()->FontSize = 12;
            label->GetComponent<Label>()->WordWrap = false;
            label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
            label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
            
            
            
            label->AddComponent<Colorable>()->Color = Colour::Black();
        }
        
        {
            label2 = root->CreateObject();
            label2->AddComponent<Transform>()->Position = {0,12};
            label2->AddComponent<Sizeable>()->Size = {120,64};
            label2->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
            label2->AddComponent<TextureComponent>(font);
            //label2->AddComponent<Mesh>();
            label2->AddComponent<Font>(font);
            //label2->AddComponent<Label>()->Text = "Cubism, is a new puzzle game from Jeppe Nielsen";
            //label2->GetComponent<Label>()->FontSize = 64;
            //label2->GetComponent<Label>()->WordWrap = true;
            
        }
        
        {
        GameObject* cube = root->CreateObject();
        cube->AddComponent<Transform>()->Position = {32,32,-1};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {32,32,0.02f});
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
    
        Input.ButtonDown.Bind([font] (std::string button) {
            font->GetComponent<Font>()->Clear();
            //std::string path = "/Projects/PocketEngine/Games/TestFreeType/test.png";
            //font->GetComponent<TextureComponent>()->Texture().SaveToPng(path, GL_RGBA);
        });
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
        
        label->GetComponent<Label>()->FontSize = Input.GetTouchPosition(0).x * 0.2f;
        //label->GetComponent<Sizeable>()->Size = { Input.GetTouchPosition(0).x * 0.1f, 100};
        
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }


    void Render() {
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};


int main() {

    Engine e;
    e.Start<DynamicFont>();
    return 0;
}
