//
//  Trigger.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Trigger.hpp"
using namespace Pocket;

Trigger::Trigger() { }

Trigger::~Trigger() {
    for (int i=0; i<variables.size(); ++i) {
        if (variables[i]) {
            delete variables[i];
        }
    }
    variables.clear();
    clone = 0;
}

Trigger::Trigger(const Trigger& other) {
    for (int i=0; i<variables.size(); ++i) {
        if (variables[i]) {
            delete variables[i];
        }
    }
    variables.clear();
    for(auto v : other.variables) {
        variables.push_back(v->Clone());
    }
    Source = other.Source;
}

void Trigger::operator=(const Pocket::Trigger& other) {
    for (int i=0; i<variables.size(); ++i) {
        if (variables[i]) {
            delete variables[i];
        }
    }
    variables.clear();
    for(auto v : other.variables) {
        variables.push_back(v->Clone());
    }
    Source = other.Source;
    clone = 0;
}

void Trigger::Invoke() {
    GameObject* source = Source();
    
    std::vector<IFieldInfo*> sourceVaribles;
    FindVariables(sourceVaribles, source);
    
    if (variables.size() == sourceVaribles.size()) {
        for (int i=0; i<variables.size(); ++i) {
            if (variables[i]->type == -1) {
                sourceVaribles[i]->SetFromAny(static_cast<FieldInfoAny*>(variables[i]));
            } else {
                sourceVaribles[i]->SetFromOther(variables[i]);
            }
        }
    }
}


void Trigger::CreateVariables(GameObject* root) {
    if (clone) {
        for(auto v : variables) {
            delete v;
        }
        variables.clear();
        clone->Remove();
        clone = 0;
    }
    GameObject* source = Source;
    if (!source) return;
    
    clone = root->CreateChildClone(source, [source] (GameObject* o) {
        return o == source;
    });
    
    std::vector<IFieldInfo*> infos;
    FindVariables(infos, clone);
    if (infos.size() == variables.size()) {
        for(int i=0; i<infos.size(); ++i) {
            if (variables[i]->type == -1) {
                infos[i]->SetFromAny(static_cast<FieldInfoAny*>(variables[i]));
            }
            delete variables[i];
        }
    }
    variables = infos;
    
    /*for(int i = 0; i<variables.size(); ++i) {
        variables[i]=variables[i]->Clone();
    }*/
}

void Trigger::FindVariables(std::vector<IFieldInfo*>& variables, Pocket::GameObject *objectWithVariable) {
    int size = (int)objectWithVariable->World()->GetComponentTypes().size();
    
    for (int i=0; i<size; ++i) {
        if (!objectWithVariable->HasComponent(i)) continue;
        TypeInfo info = objectWithVariable->GetComponentTypeInfo(i);
        for(auto field : info.fields) {
            variables.push_back(field->Clone());
        }
    }
    /*for(auto child : objectWithVariable->Children()) {
        FindVariables(variables, child);
    }*/
}
