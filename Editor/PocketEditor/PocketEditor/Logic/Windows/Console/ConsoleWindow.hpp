//
//  ConsoleWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"

namespace Pocket {
    class VirtualTreeList;
}

class ConsoleWindow : public BaseWindow {
protected:
    std::string Name() override;
    void OnInitialize() override;
    void OnCreate() override;
    Vector2 Size() override;
private:

    void Clicked(TouchData d, GameObject* node);

    GameObject* listBox;
    VirtualTreeList* treeView;
    GameObject* compilingText;
};
