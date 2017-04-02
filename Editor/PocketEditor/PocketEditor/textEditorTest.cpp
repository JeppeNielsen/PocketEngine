//
//  textEditor.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "DraggableSystem.hpp"
#include "TextEditor.hpp"
#include <fstream>
#include "TextEditorRendererSystem.hpp"
#include "FontTextureSystem.hpp"
#include "TransformHierarchy.hpp"
#include "TextEditorEditSystem.hpp"
#include "TextBoxSystem.hpp"
#include "TextEditorCursorSystem.hpp"

using namespace Pocket;

class TextEditorState : public GameState<TextEditorState> {
public:
    GameWorld world;
    GameObject* font;
    GameObject* box;
    GameObject* textEditor;
    
    int cursor;
   
    void Initialize() {
        
        
        GameObject* root = world.CreateRoot();
        root->CreateSystem<RenderSystem>()->Octree().SetWorldBounds({0,3000});
        root->CreateSystem<TouchSystem>()->Octree().SetWorldBounds({0,3000});
        root->CreateSystem<DraggableSystem>();
        root->CreateSystem<FontTextureSystem>();
        root->CreateSystem<TextEditorRendererSystem>();
        root->CreateSystem<TransformHierarchy>();
        root->CreateSystem<TextEditorEditSystem>();
        root->CreateSystem<TextBoxSystem>();
        root->CreateSystem<TextEditorCursorSystem>();
        
        font = root->CreateChild();
        font->AddComponent<TextureComponent>();
        font->AddComponent<Font>()->LoadTTF("PTMono.ttc");
        font->GetComponent<Font>()->fontSizeToPixelFactor = 1.0f;
        font->GetComponent<Font>()->CharacterSetEverySize = 12.0f;
        
        
        GameObject* camera = root->CreateObject();
        camera->AddComponent<Transform>()->Position = Vector3(0,0,1);
        Camera* cam = camera->AddComponent<Camera>();
        
        cam->Orthographic = true;
        cam->Near = 1.0f;
        cam->Far = 2.0f;
        cam->FieldOfView = 0.0f;
        
        for(int i=0; i<1; i++) {
        
        GameObject* box = root->CreateChild();
        
        Vector2 size = {500-i*400.0f,400-i*200.0f};
        
        box->AddComponent<Transform>();
        box->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(size*0.5f, size, Colour::Blue(0.5f));
        box->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        box->AddComponent<Draggable>();
        box->AddComponent<Touchable>();
        //box->AddComponent<TextureComponent>(font);
        
        std::ifstream file("/Projects/PocketEngine/Editor/PocketEditor/Build/Build/Products/Debug/output.txt");
        
        textEditor = box->CreateChild();
        textEditor->AddComponent<Transform>();
        textEditor->AddComponent<Mesh>();
        textEditor->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        textEditor->AddComponent<TextureComponent>(font);
        textEditor->AddComponent<Font>(font);
        auto& t = textEditor->AddComponent<TextEditor>()->text;
        t.assign( (std::istreambuf_iterator<char>(file) ),
                (std::istreambuf_iterator<char>()    ) );
    
        for(int j=0; j<8; j++) {
            t += "1234567891O123456781234567891O1234567891OOOO1234567891O1234567891OOOO\n";
        }
        
        textEditor->AddComponent<TextEditorRenderer>()->fontSize = 32;
        textEditor->AddComponent<Sizeable>()->Size = size;
        textEditor->AddComponent<TextBox>();
        textEditor->AddComponent<Touchable>(box);
        
        GameObject* ref = box->CreateChild();
        ref->AddComponent<Transform>()->Position = {0,0,0};
        //ref->GetComponent<Transform>()->Scale = {100,10,0.2f};
        ref->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0,{100,1,0.01f});
        ref->AddComponent<Renderable>();
        

        auto te = textEditor->GetComponent<TextEditor>();
        te->Cursor.Changed.Bind([te] () {
            if (te->Cursor<0) {
                std::cout << "wolla"<< std::endl;
            }
        });

        }
        
        cursor = 200;
        /*
        Input.ButtonDown.Bind([this] (std::string b) {
            
            
            
            std::string& text = textEditor->GetComponent<TextEditor>()->text;
            std::string t1 = text.substr(0, cursor);
            std::string t2 = text.substr(cursor, text.size() - cursor);
            
            if (b == "\r") {
                b = "\n";
            } else if (b == "\x7f") {
                t1 = t1.substr(0, t1.size()-1);
                b = "";
                cursor-=2;
            } else if (b.size()>1) {
                return;
            }
            
            textEditor->GetComponent<TextEditor>()->text = t1 + b + t2;
            textEditor->GetComponent<TextEditor>()->Lines.MakeDirty();
            
            cursor++;
        });
        */
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        //textEditor->GetComponent<TextEditorRenderer>()->Offset = {world.Input().GetTouchPosition(0).y,0};
        world.Update(dt);
    }
    
    void Render() {
        glClearColor(0.9f, 0.9f, 0.9f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<TextEditorState>();
	return 0;
}
