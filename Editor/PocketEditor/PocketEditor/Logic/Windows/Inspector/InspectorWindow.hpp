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
#include "ScriptWorld.hpp"
#include "AppMenu.hpp"
#include "GameObjectEditor.hpp"

class InspectorWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    void ActiveWorldChanged(OpenWorld* old, OpenWorld* current) override;
    void PostCompile() override;
private:
    SelectableCollection<EditorObject>* selectables;
    GameObject* inspectorEditor;
    void EditorRootChanged(OpenWorld* world);
    void ChangeEditorRoot(GameObject* old, GameObject* current);
    void SelectionChanged(SelectableCollection<EditorObject>* selectables);
    void AddComponentClicked(TouchData d);
    GameObject* listBox;
    GameObject* addComponentButton;
    
    void ShowSelectionBox(EditorObject* editorObject);
    GameObject* selectionBox;
    void SelectionClicked(TouchData d, int index);
    
    AppMenu removeComponentMenu;
    GameObjectEditor::ComponentCreatedData clickedComponent;
    
    void ComponentClicked(TouchData d, GameObjectEditor::ComponentCreatedData data);
    void RefreshInspector();
    GameObject* TryGetFirstObjectWithComponent(int componentId);
    
    int numberOfComponents;
    void PollForChanges(float dt);
};
