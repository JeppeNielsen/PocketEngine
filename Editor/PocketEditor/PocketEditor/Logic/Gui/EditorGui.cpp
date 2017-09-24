//
//  EditorGui.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "EditorGui.hpp"
#include "Gui.hpp"
#include "VirtualTreeList.hpp"

using namespace Pocket;

void EditorGui::Initialize() {
    gui = root->CreateSystem<Gui>();
}

GameObject* EditorGui::CreateTreeList(GameObject* parent, GameObject* root,
        const std::function<std::string(GameObject*)>& expandedHashFunction,
        const std::function<bool(GameObject* object)>& predicateFunction,
        const std::function<bool(GameObject*)>& hasChildren,
        const std::function<void(VirtualTreeListSpawner::SpawnedNode& node, GameObject* button, std::string& text)>& OnCreate,
        const std::function<void(const VirtualTreeListSpawner::SpawnedNode& node, GameObject* button)>& OnRemove,
        bool showRoot) {

    GameObject* pivot;
    GameObject* listBox = gui->CreateListbox(parent, "Window", {1,0}, 200, &pivot);
    listBox->RemoveComponent<Renderable>();
    
    gui->AddLayouter(listBox, 20, {2000,2000}, {2000,2000});
    
    pivot->GetComponent<Transform>()->Position = {0,400};
    
    auto treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = root;
    treeView->ShowRoot = showRoot;
    if (root) {
        treeView->SetNodeExpanded(root, true);
    }
    treeView->Pivot = listBox;
    if (expandedHashFunction) {
        treeView->ExpandedHashFunction = expandedHashFunction;
    }
    if (predicateFunction) {
        treeView->PredicateFunction = predicateFunction;
    }
    treeView->ItemHeight = 18.0f;
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    spawner->HasChildren = hasChildren;

    spawner->OnCreate = [=](auto& n) {
        
        const float height = 18.0f;
        
        GameObject* selectButton = gui->CreateControl(n.parent, "white");
        selectButton->AddComponent<TouchableCanceller>();
        selectButton->GetComponent<Touchable>()->ClickThrough = true;
        selectButton->ReplaceComponent<Sizeable>(n.parent);
        gui->AddLayouter(selectButton, {4,height}, {2000,height}, {2000,height}, Layouter::LayoutMode::Horizontal);
        
        Vector2 startPos = { 8.0f + n.depth * 16.0f , 0 };
        if (n.hasChildren) {
        
            bool isUnfolded = treeView->IsNodeExpanded(n.node);
        
            Vector2 s = isUnfolded ? Vector2(9.0f, 7.5f) : Vector2(7.5f, 9.0f);
            n.foldOutButton = gui->CreateControl(selectButton, isUnfolded ? "Icon_arrowDown" : "Icon_arrowRight",startPos + Vector2(0, height / 2 - s.y /2),s);
            
            s = {16.0f, 12.0f};
            gui->CreateControl(selectButton, "Icon_folder", startPos + Vector2(15.0f, height / 2 - s.y/2),s);
        } else {
            Vector2 s = {13.0f, 16.0f};
            gui->CreateControl(selectButton, "Icon_file", startPos + Vector2(15.0f, height / 2 - s.y/2),s);
        }
        
        auto l = gui->CreateLabel(selectButton, startPos + Vector2(40.0f, 0), {200, height}, nullptr, "", 12);
        l->GetComponent<Colorable>()->Color = Colour::Black();
        l->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
        l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        
        std::string text;
       
        selectButton->AddComponent<Selectable>()->Selected.Changed.Bind([selectButton, l] {
            l->GetComponent<Colorable>()->Color = selectButton->GetComponent<Selectable>()->Selected() ?
            Colour::White() : Colour::Black();
        });
        
        selectButton->AddComponent<SelectedColorer>()->Deselected = n.position % 2 == 0 ? Colour((Colour::Component)245,245,245,255) : Colour((Colour::Component)255,255,255,255);
        selectButton->AddComponent<SelectedColorer>()->Selected = Colour((Colour::Component)17,108,214,255);
        
        OnCreate(n, selectButton, text);
        l->GetComponent<Label>()->Text = text;
    };
    
    spawner->OnRemove = [=] (const VirtualTreeListSpawner::SpawnedNode& n) {
        OnRemove(n, n.parent->Children()[0]);
    };
    
    spawner->OnFoldoutChanged = [treeView, this] (auto& n) {
        bool isUnfolded = treeView->IsNodeExpanded(n.node);
        n.foldOutButton->template GetComponent<Sprite>()->SpriteName = isUnfolded ? "Icon_arrowDown" : "Icon_arrowRight";
        Vector2 s = isUnfolded ? Vector2(9.0f, 7.5f) : Vector2(7.5f, 9.0f);
        n.foldOutButton->template GetComponent<Sizeable>()->Size = s;
    };

    return pivot;
}
