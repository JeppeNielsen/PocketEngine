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
#include "ComponentMenu.hpp"
#include "ScriptComponentMenu.hpp"
#include "AnimationWindow.hpp"
#include "BackgroundArea.hpp"
#include "AtlasCreator.hpp"

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
    AtlasCreator atlasCreator;
    
    void Initialize() {
    
        std::string inputPath = "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/NewUI";
    std::string outputFile = "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/NewUI.tga";
    std::string outputAtlas = "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/NewUI.json";
    
        atlasCreator.CreateAtlas(
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/NewUI",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/NewUI.tga",
        "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/NewUI.json", 1024, 1024,
        {
            {"Box", {1,1,2,2} },
            {"TextBox", {1,1,1,1} },
            {"Window", {1,1,1,1} },
        });
    
        windows.push_back(new BackgroundArea());
        windows.push_back(new WorldTab());
//        windows.push_back(new SceneTab());
//        windows.push_back(new PlayButtons());
        windows.push_back(new ProjectWindow());
        windows.push_back(new HierarchyWindow());
        windows.push_back(new InspectorWindow());
        //windows.push_back(new ConsoleWindow());
        //windows.push_back(new AnimationWindow());
        
        menus.push_back(new FileMenu());
        menus.push_back(new GameObjectMenu());
        menus.push_back(new GameMenu());
        menus.push_back(new ComponentMenu());
        menus.push_back(new ScriptComponentMenu());
        
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
        
        GameObject* area = windows[0]->Window();//->GetComponent<PanelArea>();
        area->GetComponent<PanelArea>()->IsDirty = true;
        PanelLocation loc;
        loc.Push(PanelDirection::Right);
        area->GetComponent<PanelArea>()->SetSplitValue(loc.Id(), 0.333333f);
        
        Panel* p1 = windows[1]->Window()->Hierarchy().Parent()->GetComponent<Panel>();
        p1->Area = area;
        p1->location.Push(PanelDirection::Left);
        
        Panel* p2 = windows[2]->Window()->Hierarchy().Parent()->GetComponent<Panel>();
        p2->Area = area;
        p2->location.Push(PanelDirection::Right);
        p2->location.Push(PanelDirection::Right);
        p2->location.Push(PanelDirection::Left);
        
        
        Panel* p3 = windows[3]->Window()->Hierarchy().Parent()->GetComponent<Panel>();
        p3->Area = area;
        p3->location.Push(PanelDirection::Right);
        p3->location.Push(PanelDirection::Left);
        
        
        Panel* p4 = windows[4]->Window()->Hierarchy().Parent()->GetComponent<Panel>();
        p4->Area = area;
        p4->location.Push(PanelDirection::Right);
        p4->location.Push(PanelDirection::Right);
        p4->location.Push(PanelDirection::Right);
        
        context.Project().Open("/Projects/PocketEngine/EditorProjects/BallRunners");
        
        Input.ButtonDown.Bind([this](ButtonEvent e) {
            if (e.Id == "p") {
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

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}
