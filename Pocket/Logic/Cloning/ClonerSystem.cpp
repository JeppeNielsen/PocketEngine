
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
    } else {
        Cloner* cloner = object->GetComponent<Cloner>();
        std::vector<IFieldInfo*> variables;
        cloner->components.clear();
        FindVariables(variables, object, object->Children()[0]);
        cloner->variables = variables;
    }
}

void ClonerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.Changed.Unbind(this, &ClonerSystem::CloneSourceChanged, object);
}

void ClonerSystem::CloneSourceChanged(Pocket::GameObject *object) {
    Cloner* cloner = object->GetComponent<Cloner>();
    
    for(auto componentId : cloner->components) {
        object->RemoveComponent(componentId);
    }
    cloner->components.clear();
    for(auto child : object->Children()) {
        child->Remove();
    }
    GameObject* source = cloner->Source;
    if (!source) return;
    GameObject* child = object->CreateChildClone(source);
    std::vector<IFieldInfo*> variables;
    FindVariables(variables, object, child);
    if (variables.size() == cloner->variables.size()) {
        for (int i=0; i<variables.size(); ++i) {
            if (cloner->variables[i] && cloner->variables[i]->type == -1) {
                variables[i]->SetFromAny(static_cast<FieldInfoAny*>(cloner->variables[i]));
                delete cloner->variables[i];
            }
        }
    }
    cloner->variables = variables;
}

void ClonerSystem::FindVariables(std::vector<IFieldInfo*>& variables, GameObject* cloner, Pocket::GameObject *objectWithVariable) {
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
                    } else {
                        if (!cloner->HasComponent(index)) {
                            cloner->GetComponent<Cloner>()->components.push_back(index);
                            cloner->AddComponent(index, objectWithVariable);
                        }
                    }
                }
            }
        }
    }
    for(auto child : objectWithVariable->Children()) {
        FindVariables(variables, cloner, child);
    }
}

