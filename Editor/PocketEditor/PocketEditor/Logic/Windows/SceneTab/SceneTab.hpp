//
//  ProjectWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"
#include <map>

class SceneTab : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    bool CreateBar() override;
private:

    void ScreenSizeChanged();

    void ActiveWorldChanged(OpenWorld* old, OpenWorld* current) override;
    
    void IsPlayingChanged(OpenWorld* world);
    void LayersChanged(OpenWorld* world);
    
    void UpdateWorld(OpenWorld* world);
    
    void ClearTabs();
    void AlignTabs();
    
    void AddTab(OpenWorld* world, GameObject* scene);
    
    void ActiveSceneChanged(RunningWorld* world);

    void Clicked(TouchData d, OpenWorld* clickedWorld);
    GameObject* fileRoot;
    
    using OpenedTabs = std::map<GameObject*, GameObject*>;
    OpenedTabs openedTabs;
    
    GameObject* tabArea;
    GameObject* renderArea;
};
