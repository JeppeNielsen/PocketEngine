//
//  GameMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptComponentMenu.hpp"
#include "Timer.hpp"
#include "FileReader.hpp"
#include "FileHelper.hpp"
#include "ComponentHelper.hpp"

std::string ScriptComponentMenu::Name() {
    return "Custom Components";
}

void ScriptComponentMenu::OnInitialize() {
}

void ScriptComponentMenu::OnCreate() {

    context->Project().Opened.Bind(this, &ScriptComponentMenu::CompilationFinished);
    context->Project().CompilationFinished.Bind(this, &ScriptComponentMenu::CompilationFinished);
}

void ScriptComponentMenu::CompilationFinished() {
    auto components = ComponentHelper::GetSortedScriptComponents(*this->context->ContextRoot().World(), this->context->Project().ScriptWorld());

    menu->Clear();

    for(auto& category : components) {
        auto& componentMenu = menu->AddChild(category.first);
        
        for(auto& component : category.second) {
            int index = component.index;
            componentMenu.AddChild(component.name).Clicked.Bind([this, index] {
                auto selectables = this->context->Project().GetSelectables();
                if (!selectables) return;
                for(auto o : selectables->Selected()) {
                    GameObject* object = o->GetComponent<EditorObject>()->gameObject;
                    object->AddComponent(index);
                }
            });
        }
    }

}
