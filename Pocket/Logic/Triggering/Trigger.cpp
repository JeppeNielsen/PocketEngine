//
//  Trigger.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Trigger.hpp"
#include "GameWorld.hpp"
using namespace Pocket;

Trigger::Trigger() : clone(0) {}

void Trigger::Invoke() {
    GameObject* source = Source();
    if (!source) return;
    
    std::vector<std::shared_ptr<IFieldInfo>> sourceVaribles;
    FindVariables(sourceVaribles, source);
    
    if (variables.size() == sourceVaribles.size()) {
        for (int i=0; i<variables.size(); ++i) {
            if (variables[i]->type == -1) {
                sourceVaribles[i]->SetFromAny(static_cast<FieldInfoAny*>(variables[i].get()));
            } else {
                sourceVaribles[i]->SetFromOther(variables[i].get());
            }
        }
    }
}


void Trigger::CreateVariables(GameObject* root) {
    if (clone) {
        TryStoreVariables();
        clone->Remove();
        clone = 0;
    }
    GameObject* source = Source;
    if (!source) return;
    
    clone = root->CreateChildClone(source, [source] (GameObject* o) {
        return o == source;
    });
    
    std::vector<std::shared_ptr<IFieldInfo>> infos;
    FindVariables(infos, clone);
    if (infos.size() == variables.size()) {
        for(int i=0; i<infos.size(); ++i) {
            if (variables[i]->type == -1) {
                infos[i]->SetFromAny(static_cast<FieldInfoAny*>(variables[i].get()));
            }
        }
    }
    variables = infos;
}

void Trigger::FindVariables(std::vector<std::shared_ptr<IFieldInfo>>& variables, Pocket::GameObject *objectWithVariable) {
    int size = (int)objectWithVariable->World()->Storage().GetComponentTypes().size();
    
    for (int i=0; i<size; ++i) {
        if (!objectWithVariable->HasComponent(i)) continue;
        TypeInfo info = objectWithVariable->GetComponentTypeInfo(i);
        for(auto& field : info.fields) {
            variables.push_back(field->Clone());
        }
    }
    /*for(auto child : objectWithVariable->Children()) {
        FindVariables(variables, child);
    }*/
}

void Trigger::TryStoreVariables() {
    if (variables.empty()) return;
    if (variables[0]->type == -1) return;
    
    std::stringstream stream;

    auto type = GetType();
    auto fieldInfo = type.GetField("variables");
    
    minijson::writer_configuration config;
    //config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    fieldInfo->Serialize(writer);
    writer.close();

    variables.clear();
    minijson::istream_context context(stream);
    type.Deserialize(context);
}

