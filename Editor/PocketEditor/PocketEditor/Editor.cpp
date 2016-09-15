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
#include "ClickSelectorSystem.hpp"
#include "EditorTransformSystem.hpp"
#include "RenderSystem.hpp"
#include "EditorMeshSystem.hpp"
#include "ClickSelectorSystem.hpp"
#include "DragSelector.hpp"
#include "SelectableDragSystem.hpp"
#include "InspectorWindow.hpp"
#include "FileMenu.hpp"
#include "GameMenu.hpp"
#include "GameObjectMenu.hpp"
#include "ProjectWindow.hpp"
#include "WorldTab.hpp"

#include <vector>

using namespace Pocket;

class Editor : public GameState<Editor> {
public:
    EditorContext context;
    ScriptWorld scriptWorld;
    using Windows = std::vector<BaseWindow*>;
    Windows windows;
    using Menus = std::vector<BaseMenu*>;
    Menus menus;
    
    OpenWorld* worlds[2];
    
    void Initialize() {

        windows.push_back(new WorldTab());
        windows.push_back(new ProjectWindow());
        windows.push_back(new HierarchyWindow());
        windows.push_back(new InspectorWindow());
        
        menus.push_back(new FileMenu());
        menus.push_back(new GameObjectMenu());
        menus.push_back(new GameMenu());
        
        context.Initialize(Input, Context());
        
        for(auto window : windows) {
            window->Initialize(&context);
        }
        for(auto menu : menus) {
            menu->Initialize(&Context(), &context);
        }
        
        for(auto window : windows) {
            window->Create();
        }
        
        for(auto menu : menus) {
            menu->Create();
        }
        
        
        
        Input.ButtonDown.Bind([this] (auto key) {
            if (key == "n") {
                context.NewProject();
            }
        });
        
        context.NewProject();
    }
    
    void Update(float dt) {
        context.ContextWorld().Update(dt);
        context.Project().Update(dt);
        context.GuiWorld().Update(dt);
    }
    
    void Render() {
        //glClearColor(1, 1, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);
        context.Project().Render();
        context.GuiWorld().Render();
    }
};

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}