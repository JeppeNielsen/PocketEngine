//
//  GameMenu.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "BaseMenu.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"

class ComponentMenu : public BaseMenu {
protected:
    std::string Name() override;
    void OnCreate() override;
    void OnInitialize() override;
};
