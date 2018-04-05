//
//  ConsoleWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "EditorGui.hpp"

namespace Pocket {
    class VirtualTreeList;
}

class ConsoleWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
private:

    void Clicked(TouchData d, GameObject* node);

    GameObject* compilingText;
    
    VirtualTreeList* treeView;
    GameObject* rootItem;
    std::map<GameObject*, GameObject*> objectToSelectButton;
    std::map<GameObject*, GameObject*> objectToEnableButton;
    EditorGui* editorGui;
};
