
//
//  ClonerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ClonerSystem.hpp"
#include "GameWorld.hpp"
#include "CloneVariable.hpp"
#include <fstream>

using namespace Pocket;

void ClonerSystem::Initialize() {
    root->World()->AddComponentType<CloneVariable>();
}

void ClonerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.SetRoot(root);
    object->GetComponent<Cloner>()->Source.Changed.Bind(this, &ClonerSystem::CloneSourceChanged, object);
    if (object->Children().empty()) {
        CloneSourceChanged(object);
    }
}

void ClonerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.Changed.Unbind(this, &ClonerSystem::CloneSourceChanged, object);
    for(auto child : object->Children()) {
        child->Remove();
    }
}

void ClonerSystem::CloneSourceChanged(Pocket::GameObject *object) {
    Cloner* cloner = object->GetComponent<Cloner>();
    for(auto child : object->Children()) {
        child->Remove();
    }
    GameObject* source = cloner->Source;
    if (!source) return;
    GameObject* child = object->CreateChildClone(source);
    std::vector<IFieldInfo*> variables;
    FindVariables(variables, child);
    if (variables.size() == cloner->variables.size()) {
        for (int i=0; i<variables.size(); ++i) {
            if (cloner->variables[i]->type == -1) {
                variables[i]->SetFromAny(static_cast<FieldInfoAny*>(cloner->variables[i]));
                delete cloner->variables[i];
            }
        }
    }
    cloner->variables = variables;
}

void ClonerSystem::FindVariables(std::vector<IFieldInfo*>& variables, Pocket::GameObject *objectWithVariable) {
    CloneVariable* vars = objectWithVariable->GetComponent<CloneVariable>();
    
    if (vars) {
        for(auto& var : vars->Variables ) {
            int index;
            if (root->World()->TryGetComponentIndex(var.componentName, index)) {
                if (objectWithVariable->HasComponent(index)) {
                    auto typeInfo = objectWithVariable->GetComponentTypeInfo(index);
                    auto fieldInfo = typeInfo.GetField(var.fieldName);
                    if (fieldInfo) {
                        variables.push_back(fieldInfo->Clone());
                    }
                }
            }
        }
    }
    for(auto child : objectWithVariable->Children()) {
        FindVariables(variables, child);
    }
}

