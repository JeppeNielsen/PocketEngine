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
#include "TextEditorSelectionMeshSystem.hpp"
#include "TextEditorSelectionSystem.hpp"
#include "TextEditorScrollSystem.hpp"
#include "AutocompleterEntrySystem.hpp"
#include "AutocompleterTextEditorSystem.hpp"
#include "Gui.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"

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
        root->CreateSystem<Gui>()->Setup("images.png", "images.xml", Context().Viewport());
        root->CreateSystem<Gui>()->CreateFont("/Library/Fonts/Arial Bold.ttf");//, "Font");
    
        //root->CreateSystem<RenderSystem>()->Octree().SetWorldBounds({0,3000});
        //root->CreateSystem<TouchSystem>()->Octree().SetWorldBounds({0,3000});
        //root->CreateSystem<DraggableSystem>();
        //root->CreateSystem<FontTextureSystem>();
        root->CreateSystem<TextEditorRendererSystem>();
        //root->CreateSystem<TransformHierarchy>();
        root->CreateSystem<TextEditorEditSystem>();
        //root->CreateSystem<TextBoxSystem>();
        root->CreateSystem<TextEditorCursorSystem>();
        root->CreateSystem<TextEditorSelectionMeshSystem>();
        root->CreateSystem<TextEditorSelectionSystem>();
        root->CreateSystem<TextEditorScrollSystem>();
        root->CreateSystem<AutocompleterEntrySystem>();
        root->CreateSystem<AutocompleterTextEditorSystem>();
        root->CreateSystem<VirtualTreeListSystem>();
        root->CreateSystem<VirtualTreeListSpawnerSystem>();
        
        
        font = root->CreateChild();
        font->AddComponent<TextureComponent>();
        font->AddComponent<Font>()->LoadTTF("PTMono.ttc");
        font->GetComponent<Font>()->fontSizeToPixelFactor = 1.0f;
        font->GetComponent<Font>()->CharacterSetEverySize = 12.0f;
        
        
        /*GameObject* camera = root->CreateObject();
        camera->AddComponent<Transform>()->Position = Vector3(0,0,1);
        Camera* cam = camera->AddComponent<Camera>();
        
        cam->Orthographic = true;
        cam->Near = 1.0f;
        cam->Far = 2.0f;
        cam->FieldOfView = 0.0f;
        */
        
        GameObject* autoCompleter = root->CreateChild();
        
        autoCompleter->AddComponent<Autocompleter>()->sourceFile = "/Projects/PocketEngine/EditorProjects/AutoComplete/main.cpp";
        autoCompleter->AddComponent<AutocompleterEntry>();
        
        box = root->CreateChild();
        
        Vector2 size = Vector2(1280, 720);
        
        box->AddComponent<Transform>();
        box->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(size*0.5f, size, Colour::Blue(0.5f));
        box->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        //box->AddComponent<Draggable>();
        box->AddComponent<Touchable>();
        box->AddComponent<Orderable>();
        //box->AddComponent<TextureComponent>(font);
        
        std::ifstream file("/Projects/PocketEngine/EditorProjects/AutoComplete/main.cpp");
        
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
        
        textEditor->AddComponent<TextEditorRenderer>()->fontSize = 12;
        textEditor->AddComponent<Sizeable>()->Size = size;
        textEditor->AddComponent<TextBox>();
        textEditor->AddComponent<Touchable>(box);
        textEditor->AddComponent<Autocompleter>(autoCompleter);
        textEditor->AddComponent<Orderable>();
        
        
        
    }
    
    void Update(float dt) {
        //textEditor->GetComponent<Sizeable>()->Size = Context().ScreenSize;
    
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
