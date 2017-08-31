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
#include "VirtualTreeList.hpp"
#include "EditorGui.hpp"

class HierarchyWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    void ActiveWorldChanged(OpenWorld* old, OpenWorld* current) override;
private:
    void GameSceneChanged(OpenWorld* world);
    void Clicked(TouchData d, GameObject* object);
    void Dropped(DroppedData d, GameObject* object);
    bool IsParentValid(GameObject* object, GameObject* possibleParent);
    
    void SelectedChanged(GameObject* object);
    void SetNodeSelected(GameObject* node, bool selected);
    
    void EnabledChanged(GameObject* object);
    void SetNodeEnabled(GameObject* node, bool enabled);
    
    VirtualTreeList* treeView;
    GameObject* rootItem;
    std::map<GameObject*, GameObject*> objectToSelectButton;
    std::map<GameObject*, GameObject*> objectToEnableButton;
    EditorGui* editorGui;
};
