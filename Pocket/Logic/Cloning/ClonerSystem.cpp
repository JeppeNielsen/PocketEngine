
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

void ClonerSystem::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddComponentType<CloneVariable>();
}

void ClonerSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.Changed.Bind(this, &ClonerSystem::CloneSourceChanged, object);
    if (object->Hierarchy().Children().empty()) {
        CloneSourceChanged(object);
    } else {
        Cloner* cloner = object->GetComponent<Cloner>();
        std::vector<std::shared_ptr<IFieldInfo>> variables;
        cloner->components.clear();
        FindVariables(variables, object, object->Hierarchy().Children()[0]);
        cloner->variables = variables;
    }
}

void ClonerSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Cloner>()->Source.Changed.Unbind(this, &ClonerSystem::CloneSourceChanged, object);
}

void ClonerSystem::CloneSourceChanged(Pocket::GameObject *object) {
    //std::cout << "Cloner changed : " << object->RootGuid() << "  :  " << object->RootId() << std::endl;
    Cloner* cloner = object->GetComponent<Cloner>();
    
    cloner->TryStoreVariables();
    
    for(auto componentId : cloner->components) {
        object->RemoveComponent(componentId);
    }
    cloner->components.clear();
    for(auto child : object->Hierarchy().Children()) {
        child->Remove();
    }
    GameObject* source = cloner->Source;
    if (!source) return;
    //source->Enabled = false;
    GameObject* child = object->CreateChildClone(source);
    std::vector<std::shared_ptr<IFieldInfo>> variables;
    FindVariables(variables, object, child);
    if (variables.size() == cloner->variables.size()) {
        for (int i=0; i<variables.size(); ++i) {
            //std::cout << "Cloner Want to remove variable:  " << &variables[i] << "  " << object->RootId() << std::endl;
            //std::cout << cloner->variables[i]->ToString() << "  " << std::endl;
            if (cloner->variables[i] && cloner->variables[i]->type == -1) {
                variables[i]->SetFromAny(static_cast<FieldInfoAny*>(cloner->variables[i].get()));
                //std::cout << "Cloner  Remove variable:  " << cloner->variables[i] << "  " << object->RootId() << std::endl;
                cloner->variables[i] = 0;
            }
        }
    }
    cloner->variables = variables;
}

void ClonerSystem::FindVariables(std::vector<std::shared_ptr<IFieldInfo>>& variables, GameObject* cloner, Pocket::GameObject *objectWithVariable) {
    CloneVariable* vars = objectWithVariable->GetComponent<CloneVariable>();
    
    if (vars) {
        for(auto& var : vars->Variables ) {
            int index;
            if (root->World()->Storage().TryGetComponentIndex(var.componentName, index)) {
                if (objectWithVariable->HasComponent(index)) {
                    auto typeInfo = objectWithVariable->GetComponentTypeInfo(index);
                    auto fieldInfo = typeInfo.GetField(var.fieldName);
                    if (fieldInfo) {
                        variables.push_back(fieldInfo->Clone());
                    } else {
                        cloner->GetComponent<Cloner>()->components.push_back(index);
                        cloner->ReplaceComponent(index, objectWithVariable);
                    }
                }
            }
        }
    }
    for(auto child : objectWithVariable->Hierarchy().Children()) {
        FindVariables(variables, cloner, child);
    }
}
