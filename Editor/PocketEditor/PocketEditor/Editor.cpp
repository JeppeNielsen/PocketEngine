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
#include "PlayButtons.hpp"
#include "Guid.hpp"
#include "BaseModule.hpp"
#include "AssetImporters.hpp"
#include "SceneTab.hpp"
#include <vector>
#include "ConsoleWindow.hpp"

using namespace Pocket;

class Editor : public GameState<Editor> {
public:
    EditorContext context;
    ScriptWorld scriptWorld;
    using Windows = std::vector<BaseWindow*>;
    Windows windows;
    using Menus = std::vector<BaseMenu*>;
    Menus menus;
    using Modules = std::vector<BaseModule*>;
    Modules modules;
    
    void Initialize() {

        windows.push_back(new WorldTab());
        windows.push_back(new SceneTab());
        windows.push_back(new PlayButtons());
        windows.push_back(new ProjectWindow());
        windows.push_back(new HierarchyWindow());
        windows.push_back(new InspectorWindow());
        windows.push_back(new ConsoleWindow());
        
        menus.push_back(new FileMenu());
        menus.push_back(new GameObjectMenu());
        menus.push_back(new GameMenu());
        
        modules.push_back(new AssetImporters());
        
        context.Initialize(Context());
        
        for(auto window : windows) {
            window->Initialize(&context);
        }
        for(auto menu : menus) {
            menu->Initialize(&Context(), &context);
        }
        for(auto module : modules) {
            module->Initialize(&Context(), &context);
        }
        
        for(auto window : windows) {
            window->Create();
        }
        
        for(auto menu : menus) {
            menu->Create();
        }
        
        for(auto module : modules) {
            module->Create();
        }
        
        context.Project().Open("/Projects/PocketEngine/EditorProjects/Pong");
        
        Input.ButtonDown.Bind([this](std::string button) {
            if (button == "p") {
                //context.World().DebugSystems();
                context.Log().Log("Test message");
            }
        });
        
        
        std::string header = "/Project/main.hpp";
        
        std::string line =   "/Project/main.hpp:4:3: error";
        
        
        size_t found = line.find(header);
        if (found != std::string::npos) {
            size_t lineNo = found + header.size() + 1;
            size_t column = line.find(":", lineNo);
            size_t columnEnd = line.find(":", column+1);
            
            if (lineNo!=std::string::npos &&
                column!=std::string::npos &&
                columnEnd!=std::string::npos) {
            
                std::string lineNoStr = line.substr(lineNo, column - lineNo);
                std::string columnStr = line.substr(column+1, columnEnd - column-1);
                std::string errorStr = line.substr(columnEnd+1, line.size() - columnEnd-1);
                
                std::cout << lineNoStr << std::endl;
                std::cout << columnStr << std::endl;
                std::cout << errorStr << std::endl;
            }
        }
        
        
    }
    
    void Update(float dt) {
        context.Update(dt);
    }
    
    void Render() {
        context.Render();
    }
};

int main_editor() {
    Engine e;
    e.Start<Editor>();
	return 0;
}
