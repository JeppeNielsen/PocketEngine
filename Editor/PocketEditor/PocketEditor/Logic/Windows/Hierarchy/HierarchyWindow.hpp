//
//  HierarchyWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "SelectableFactory.hpp"
#include "SelectableCollection.hpp"

class HierarchyWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
private:
    void Clicked(TouchData d, GameObject* object);
    SelectableFactory* factory;
    SelectableCollection* selectables;
};