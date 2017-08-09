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

class WorldTab : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    bool CreateBar() override;
private:

    void WorldLoaded(OpenWorld* world);
    void WorldClosed(OpenWorld* world);
    void ActiveWorldChanged(OpenWorld* old, OpenWorld* current) override;

    void Clicked(TouchData d, OpenWorld* clickedWorld);
    GameObject* fileRoot;
    
    using OpenedTabs = std::map<OpenWorld*, GameObject*>;
    OpenedTabs openedTabs;
    
    GameObject* tabArea;
    GameObject* renderArea;
    
};
