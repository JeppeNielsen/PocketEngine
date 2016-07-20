//
//  CompilationWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "InspectorWindow.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "EditorObject.hpp"
#include "FieldEditorSystem.hpp"
#include "GameObjectEditorSystem.hpp"
#include "SerializedFieldEditors.hpp"
#include "Project.hpp"

std::string InspectorWindow::Name() { return "Inspector"; }

void InspectorWindow::OnInitialize() {
    GameWorld& world = context->World();
    selectables = world.CreateSystem<SelectableCollection<EditorObject>>();
    selectables->SelectionChanged.Bind(this, &InspectorWindow::SelectionChanged);
    
    GameWorld& guiWorld = context->GuiWorld();
    
    guiWorld.CreateSystem<FieldEditorSystem>()->gui = &context->Gui();
    guiWorld.CreateSystem<GameObjectEditorSystem>()->gui = &context->Gui();
    
    guiWorld.CreateSystem<LayoutSystem>();
    
    CreateDefaultSerializedEditors();
}

void InspectorWindow::OnCreate() {

/*

    GameWorld& world = context->Project().World();
    GameObject* go = world.CreateObject();
 
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {2,1,1});
    go->AddComponent<Material>();
    go->AddComponent<EditorObject>();
    go->AddComponent<Rotator>();

*/

    GameWorld& guiWorld = context->GuiWorld();
    Gui& gui = context->Gui();
    
    guiWorld.CreateSystem<FieldEditorSystem>()->gui = &gui;
    GameObjectEditorSystem* gameObjectEditorSystem = guiWorld.CreateSystem<GameObjectEditorSystem>();
    gameObjectEditorSystem->gui = &gui;
    gameObjectEditorSystem->scriptWorld = &context->Project().ScriptWorld();
    
    addComponentButton = gui.CreateLabelControl(window, "Box", {0,400-40}, 40, 0, "+", 10);
    addComponentButton->GetComponent<Touchable>()->Click.Bind(this, &InspectorWindow::AddComponentClicked);
    addComponentButton->Enabled() = false;
    
    GameObject* pivot;
    listBox = gui.CreateListbox(window, "Box", {0,0}, {200,400-80}, &pivot);
    listBox->RemoveComponent<Sprite>();
    
    
    inspectorEditor = gui.CreatePivot(pivot);
    inspectorEditor->AddComponent<Sizeable>()->Size = {200,400};
    inspectorEditor->AddComponent<GameObjectEditor>()->Object = 0;
    
    selectionBox = 0;
}

void InspectorWindow::SelectionChanged(SelectableCollection<EditorObject> *selectables) {
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = selectables->Selected().empty() ? 0 : selectables->Selected()[0]->GetComponent<EditorObject>()->gameObject;
    addComponentButton->Enabled() = !selectables->Selected().empty();
}

void InspectorWindow::AddComponentClicked(Pocket::TouchData d) {
    if (!selectionBox) {
        ShowSelectionBox(selectables->Selected()[0]->GetComponent<EditorObject>());
    } else {
        selectionBox->Remove();
        selectionBox = 0;
    }
}

void InspectorWindow::ShowSelectionBox(EditorObject *editorObject) {
    Gui& gui = context->Gui();

    selectionBox = gui.CreateControl(window);
    
    auto& componentTypes = context->Project().World().ComponentTypes();
    
    Vector2 pos;
    for(int i=0; i<componentTypes.size(); ++i) {
        auto& componentType = componentTypes[i];
        if (!componentType.getTypeInfo) continue;
        if (editorObject->gameObject->GetComponent(i)) continue;
        
        GameObject* button = gui.CreateLabelControl(selectionBox, "Box", pos, {200,20}, 0, componentType.name, 20);
        button->GetComponent<Touchable>()->Click.Bind(this, &InspectorWindow::SelectionClicked, i);
        pos += {0,-20};
    }
}

void InspectorWindow::SelectionClicked(TouchData d, int index) {
    selectionBox->Remove();
    selectionBox = 0;
    selectables->Selected()[0]->GetComponent<EditorObject>()->gameObject->AddComponent(index);
    
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = 0;
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = selectables->Selected()[0]->GetComponent<EditorObject>()->gameObject;
}