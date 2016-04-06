//
//  Editor.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "ScriptWorld.hpp"
#include "EditorContext.hpp"
#include "HierarchyWindow.hpp"
#include "ProjectWindow.hpp"

#include <vector>

using namespace Pocket;

class Editor : public GameState<Editor> {
public:
    EditorContext context;
    ScriptWorld scriptWorld;
    using Windows = std::vector<BaseWindow*>;
    Windows windows;
    
    void Initialize() {
        
        /*
        scriptWorld.SetFiles(
        "ScriptExample.so",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptInclude",
        {
            "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/ScriptCode/ScriptExample.hpp"
        },
        {
            "/Projects/PocketEngine/Pocket/Logic/Spatial/Transform.hpp",
            "/Projects/PocketEngine/Pocket/Data/Property.hpp",
            "/Projects/PocketEngine/Pocket/Math/Vector3.hpp",
            "/Projects/PocketEngine/Pocket/Logic/Gui/Sizeable.hpp",
        }
        );
        */
        
        context.NewProject();
        
        
        windows.push_back(new HierarchyWindow());
        windows.push_back(new ProjectWindow());
        
        for(auto window : windows) {
            window->Initialize(&context);
        }
        context.Initialize(Input);
        
        
        for(auto window : windows) {
            window->Create();
        }
        
        
        Input.ButtonDown.Bind([this] (auto key) {
            if (key == "n") {
                context.NewProject();
            }
        });
    }
    
//    void Compile() {
//        std::cout<<"Compilation Started..."<<std::endl;
//    
//        scriptWorld.RemoveGameWorld(world);
//        scriptWorld.Build();
//        scriptWorld.AddGameWorld(world);
//        /*
//        GameObject* window = gui->CreateControl(0, "Box", {200,200});
//        window->AddComponent<Draggable>();
//        window->GetComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,0.5f);
//        window->GetComponent<Transform>()->Position = {300,300};
//        window->AddScriptComponent(1);
//        */
//        
//        std::cout<<"Compilation Ended..." <<std::endl;
//    }
    
    void Update(float dt) {
        
        context.World().Update(dt);
        context.Project().World().Update(dt);
    }
    
    void Render() {
        //glClearColor(1, 1, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);
        context.Project().World().Render();
        context.World().Render();
    }
};

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}