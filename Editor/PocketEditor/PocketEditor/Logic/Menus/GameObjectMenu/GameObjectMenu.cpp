//
//  GameMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectMenu.hpp"
#include "Timer.hpp"

std::string GameObjectMenu::Name() {
    return "GameObject";
}

void GameObjectMenu::OnInitialize() {

}

void GameObjectMenu::OnCreate() {
    
    menu->AddChild("New", "N").Clicked.Bind([this] {
        auto object = context->Project().Worlds.ActiveWorld()->Root()->CreateObject();
        object->AddComponent<Transform>()->Position = {0,0,0};
        object->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        object->AddComponent<Material>();
        object->AddComponent<EditorObject>();
    });
    menu->AddChild("Delete", "D").Clicked.Bind([this] {
        selectables = context->Project().GetSelectables();
        for(auto o : selectables->Selected()) {
            o->GetComponent<EditorObject>()->gameObject->Remove();
        }
    });
    menu->AddChild("Clone", "C").Clicked.Bind([this] {
        selectables = context->Project().GetSelectables();
        for(auto o : selectables->Selected()) {
            GameObject* source = o->GetComponent<EditorObject>()->gameObject;
            if (source->IsRoot()) continue;
            source->CreateCopy();
         }
    });
}