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
#include "ClickSelectorSystem.hpp"
#include "EditorTransformSystem.hpp"
#include "RenderSystem.hpp"
#include "EditorMeshSystem.hpp"
#include "ClickSelectorSystem.hpp"
#include "DragSelector.hpp"
#include "SelectableDragSystem.hpp"
#include "CompilationWindow.hpp"
#include "InspectorWindow.hpp"

#include <vector>

using namespace Pocket;

class Editor : public GameState<Editor> {
public:
    EditorContext context;
    ScriptWorld scriptWorld;
    using Windows = std::vector<BaseWindow*>;
    Windows windows;
    
    void Initialize() {

        GameWorld& world = context.World();
        
        context.World().CreateSystem<RenderSystem>();
        context.World().CreateSystem<TouchSystem>()->Input = &Input;
        context.World().CreateSystem<DraggableSystem>();
        context.World().CreateSystem<EditorTransformSystem>();
        context.World().CreateSystem<EditorMeshSystem>();
        context.World().CreateSystem<ClickSelectorSystem>();
        context.World().CreateSystem<DragSelector>()->Setup(Context().ScreenSize(), Input);
        context.World().CreateSystem<SelectableDragSystem>();
        context.World().CreateSystem<TouchSystem>()->TouchDepth = 5;
        
        windows.push_back(new HierarchyWindow());
        windows.push_back(new ProjectWindow());
        windows.push_back(new CompilationWindow());
        windows.push_back(new InspectorWindow());
        
        for(auto window : windows) {
            window->Initialize(&context);
        }
        context.Initialize(Input);
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 70;
        
        context.NewProject();
        
        for(auto window : windows) {
            window->Create();
        }
        
        Input.ButtonDown.Bind([this] (auto key) {
            if (key == "n") {
                context.NewProject();
            }
        });
        
        {
            auto& menu = Context().Menu().AddChild("Project");
                menu.AddChild("New").Clicked.Bind([] () {
                    std::cout << " new Project"<<std::endl;
                });
                menu.AddChild("Open").Clicked.Bind([] () {
                    std::cout << " open project"<<std::endl;
                });
            
        }
        
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
        
        context.GuiWorld().Update(dt);
        context.Project().World().Update(dt);
        context.World().Update(dt);
    }
    
    void Render() {
        //glClearColor(1, 1, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);
        context.Project().World().Render();
        context.World().Render();
        context.GuiWorld().Render();
    }
};

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}