//
//  HierarchyWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "HierarchyWindow.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"

std::string HierarchyWindow::Name() { return "Hierarchy"; }

void HierarchyWindow::OnInitialize() {

    GameWorld& world = context->World();

    world.CreateSystem<VirtualTreeListSystem>();
    world.CreateSystem<VirtualTreeListSpawnerSystem>();


}

void HierarchyWindow::OnCreate() {
    GameWorld& world = context->World();
    Gui& gui = context->Gui();

    GameObject* pivot;
    GameObject* listBox = gui.CreateListbox(window, "Box", {0,0}, {200,800-50}, &pivot);
    listBox->RemoveComponent<Sprite>();
    
    GameObject* root = world.CreateObject();
    //root->ID = "Root";
    
    /*
    GameObject* child1 = world.CreateObject();
    child1->ID = "Child0";
    
    GameObject* child2 = world.CreateObject();
    child2->ID = "Child1";
    
    child1->Parent = root;
    child2->Parent = root;
    */
    
    /*
    for (int i=0; i<5; i++) {
        GameObject* child = world.CreateObject();
        std::stringstream s;
        s<<"Root/Child"<<i;
        std::string str =s.str();
        child->ID = str;
        child->Parent = root;
        
        GameObject* sub = world.CreateObject();
        std::stringstream ss;
        ss<<"Root/Child"<<i<<"/Sub";
        sub->ID = ss.str();
        sub->Parent = child;
    }
    */
    
    
    for(int i=0; i<100; i++) {
        GameObject* child1 = world.CreateObject();
        child1->Parent = root;
        //child1->ID = "Child1";
    
        for(int j=0; j<50; j++) {
        GameObject* child2 = world.CreateObject();
        child2->Parent = child1;
        //child2->ID = "Child2";
    
        GameObject* child3 = world.CreateObject();
        child3->Parent = child2;
        //child3->ID = "Child3";
    
            GameObject* sub1 = world.CreateObject();
            sub1->Parent = child3;
            //sub1->ID = "Sub1";
            GameObject* sub2 = world.CreateObject();
            sub2->Parent = child3;
            //sub2->ID = "Sub2";
        GameObject* child4 = world.CreateObject();
        child4->Parent = child1;
        //child4->ID = "Child4";
        
        }
    }
    
    auto treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = root;
    treeView->SetNodeExpanded(root, true);
    //treeView->SetNodeExpanded(child3, true);
    treeView->Pivot = listBox;
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->OnCreate = [&](GameObject* node, GameObject* parent) -> GameObject* {
        GameObject* clone = gui.CreateControl(parent, "Box", {-1000,0}, {200,25});
        clone->RemoveComponent<Touchable>();
        gui.CreateControl(clone, "TextBox", 0, {25,25});
        gui.CreateTextBox(clone, "TextBox", {25,0}, {200-25,25}, 0, "Object", 10);
        return clone;
    };
}
