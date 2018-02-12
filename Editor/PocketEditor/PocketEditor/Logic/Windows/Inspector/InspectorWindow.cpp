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
#include "Project.hpp"

std::string InspectorWindow::Name() { return "Inspector"; }

void InspectorWindow::OnInitialize() {

    numberOfComponents = 0;
    
    //selectables = world.CreateSystem<SelectableCollection<EditorObject>>();
    //selectables->SelectionChanged.Bind(this, &InspectorWindow::SelectionChanged);
    
    GameObject& guiRoot = context->GuiRoot();
    
    guiRoot.GetSystem<FieldEditorSystem>()->gui = &context->Gui();
    guiRoot.GetSystem<GameObjectEditorSystem>()->gui = &context->Gui();
    guiRoot.GetSystem<GameObjectEditorSystem>()->Predicate = [] (int componentId) -> bool {
        return componentId != GameIdHelper::GetComponentID<EditorObject>();
    };
    
    //guiRoot.GetSystem<LayoutSystem>();

    selectables = 0;
    
    removeComponentMenu.InitializePopUp();
    removeComponentMenu.AddChild("Add Reference").Clicked.Bind([this] () {
        if (!currentWorld) return;
        int componentIndex;
        if (!context->Storage().TryGetComponentIndex(clickedComponent.typeInfo.name, componentIndex)) {
            return;
        }
        
        GameObject* referenceObject = TryGetFirstObjectWithComponent(componentIndex);
        if (referenceObject) {
            for(auto o : selectables->Selected()) {
                o->GetComponent<EditorObject>()->gameObject->ReplaceComponent(componentIndex, referenceObject);
            }
        }
        
        context->postActions.emplace_back([this] () {
            RefreshInspector();
        });
    });
    removeComponentMenu.AddChild("Remove").Clicked.Bind([this] () {
        if (!currentWorld) return;
        int componentIndex;
        if (!context->Storage().TryGetComponentIndex(clickedComponent.typeInfo.name, componentIndex)) {
            return;
        }
        for(auto o : selectables->Selected()) {
            o->GetComponent<EditorObject>()->gameObject->RemoveComponent(componentIndex);
        }
        
        context->postActions.emplace_back([this] () {
            RefreshInspector();
        });
    });
    
    context->UpdateLoop.Bind(this, &InspectorWindow::PollForChanges);
}

void InspectorWindow::ActiveWorldChanged(OpenWorld* old, OpenWorld* current) {
    if (old) {
        old->EditorRoot.Changed.Unbind(this, &InspectorWindow::EditorRootChanged, old);
    }
    if (current) {
        current->EditorRoot.Changed.Bind(this, &InspectorWindow::EditorRootChanged, current);
    }
    ChangeEditorRoot(old ? old->EditorRoot : 0, current ? current->EditorRoot : 0);
}

void InspectorWindow::EditorRootChanged(OpenWorld *world) {
    ChangeEditorRoot(world->EditorRoot.PreviousValue(), world->EditorRoot);
}

void InspectorWindow::ChangeEditorRoot(Pocket::GameObject *old, Pocket::GameObject *current) {
    if (old) {
        old->GetSystem<SelectableCollection<EditorObject>>()->SelectionChanged.Unbind(this, &InspectorWindow::SelectionChanged);
    }
    if (current) {
        selectables = current->GetSystem<SelectableCollection<EditorObject>>();
        selectables->SelectionChanged.Bind(this, &InspectorWindow::SelectionChanged);
    } else {
        selectables = 0;
    }
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = 0;
}

void InspectorWindow::OnCreate() {

    GameObject& guiWorld = context->GuiRoot();
    Gui& gui = context->Gui();
    
    guiWorld.GetSystem<FieldEditorSystem>()->gui = &gui;
    GameObjectEditorSystem* gameObjectEditorSystem = guiWorld.GetSystem<GameObjectEditorSystem>();
    gameObjectEditorSystem->gui = &gui;
    gameObjectEditorSystem->scriptWorld = &context->ScriptWorld();
    
    addComponentButton = gui.CreateLabelControl(window, "Box", {0,400-40}, 40, 0, "+", 10);
    addComponentButton->GetComponent<Touchable>()->Click.Bind(this, &InspectorWindow::AddComponentClicked);
    addComponentButton->Hierarchy().Enabled = false;
    
    GameObject* space = gui.CreatePivot(window);
    space->AddComponent<Sizeable>();
    gui.AddLayouter(space, 20, 2000, 2000);
    
    GameObject* pivot;
    listBox = gui.CreateListbox(window, "Box", {0,0}, {200,400-80}, &pivot);
    gui.AddLayouter(listBox, 25, 2000, 2000);
    listBox->RemoveComponent<Renderable>();
    
    inspectorEditor = pivot;
    gui.AddLayouter(pivot, 25, 2000, 2000);
    //inspectorEditor->AddComponent<Sizeable>()->Size = {300,400};
    //gui.AddLayouter(inspectorEditor, 25, 2000, 2000);
    inspectorEditor->AddComponent<GameObjectEditor>()->Object = nullptr;
    inspectorEditor->GetComponent<GameObjectEditor>()->ComponentEditorCreated.Bind(
    [this](GameObjectEditor::ComponentCreatedData data) {
        data.editorPivot->GetComponent<Touchable>()->Click.Bind(this, &InspectorWindow::ComponentClicked, data);
    });
    
    selectionBox = 0;
    
    window->GetComponent<Transform>()->Position += {600,200};
}

void InspectorWindow::ComponentClicked(Pocket::TouchData d, GameObjectEditor::ComponentCreatedData data) {
    if (d.Index != 1) return;
    clickedComponent = data;
    removeComponentMenu.ShowPopup(d.Input->GetTouchPosition(d.Index));
}

void InspectorWindow::SelectionChanged(SelectableCollection<EditorObject> *selectables) {
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = selectables->Selected().empty() ? nullptr : selectables->Selected()[0]->GetComponent<EditorObject>()->gameObject;
    addComponentButton->Hierarchy().Enabled = !selectables->Selected().empty();
    numberOfComponents =  inspectorEditor->GetComponent<GameObjectEditor>()->Object() ?  inspectorEditor->GetComponent<GameObjectEditor>()->Object()->ComponentCount() : 0;
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
    
    auto componentTypes = context->Storage().GetComponentTypes();
    
    Vector2 pos;
    pos.x = window->GetComponent<Sizeable>()->Size().x;
    pos.y = window->GetComponent<Sizeable>()->Size().y;
    for(int i=0; i<componentTypes.size(); ++i) {
        auto& componentType = componentTypes[i];
        if (editorObject->gameObject->GetComponent(i)) continue;
        //if (!componentType.getTypeInfo) continue;
        
        GameObject* button = gui.CreateLabelControl(selectionBox, "Box", pos, {200,20}, 0, componentType.name, 20);
        button->GetComponent<Touchable>()->Click.Bind(this, &InspectorWindow::SelectionClicked, i);
        pos += {0,-20};
    }
}

void InspectorWindow::SelectionClicked(TouchData d, int index) {
    selectionBox->Remove();
    selectionBox = 0;
    selectables->Selected()[0]->GetComponent<EditorObject>()->gameObject->AddComponent(index);
    RefreshInspector();
}

void InspectorWindow::RefreshInspector() {
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = 0;
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = selectables->Selected()[0]->GetComponent<EditorObject>()->gameObject;
}

void InspectorWindow::PostCompile() {
    inspectorEditor->GetComponent<GameObjectEditor>()->Object = 0;
}

GameObject* InspectorWindow::TryGetFirstObjectWithComponent(int componentId) {
    
    std::vector<std::string> guids;
    std::vector<std::string> paths;
    context->Storage().GetPaths(guids, paths);
    for (int i=0; i<paths.size(); ++i) {
        
        std::ifstream file;
        file.open(paths[i]);
            
        
        //std::cout << "Start parse file: "<<p.second<<std::endl;
        
        
        //context.World().TryParseJson(file, GameIdHelper::GetComponentID<Transform>(), [] (int parent, int object) {
        //    std::cout << " parent: " << parent << "  object: " << object << std::endl;
        //});

        bool objectFound = false;
        int objectId = 0;
        context->Storage().TryParseComponent(file, componentId, [&](int parentId, int id) {
            objectFound = true;
            objectId = id;
        });
        
        if (objectFound) {
            GameObject* object = context->Storage().TryGetPrefab(guids[i]);
            if (object) {
                return object;
            }
        }
    }
    
    return 0;
}

void InspectorWindow::PollForChanges(float dt) {
    GameObject* object = inspectorEditor->GetComponent<GameObjectEditor>()->Object();
    if (!object) return;
    int currentComponentCount = object->ComponentCount();
    
    if (numberOfComponents!=currentComponentCount) {
        currentComponentCount = numberOfComponents;
        RefreshInspector();
    }
}
