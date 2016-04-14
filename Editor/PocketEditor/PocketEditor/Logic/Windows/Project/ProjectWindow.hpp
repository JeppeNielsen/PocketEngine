//
//  HierarchyWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"

class ProjectWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
private:
    SelectableCollection<EditorObject>* selectables;
};