//
//  Cloner.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Cloner.hpp"

using namespace Pocket;

Cloner::Cloner() { }

Cloner::~Cloner() {
    for (int i=0; i<variables.size(); ++i) {
        if (variables[i]) {
            delete variables[i];
        }
    }
    variables.clear();
}

Cloner::Cloner(const Cloner& other) {
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
    components = other.components;
}

void Cloner::operator=(const Pocket::Cloner& other) {
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
    components = other.components;

}

void Cloner::TryStoreVariables() {
    if (variables.empty()) return;
    if (variables[0]->type == -1) return;
    
    std::stringstream stream;

    auto type = GetType();
    IFieldInfo* fieldInfo = type.GetField("variables");
    
    minijson::writer_configuration config;
    //config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    fieldInfo->Serialize(writer);
    writer.close();

    for (int i=0; i<variables.size(); ++i) {
        delete variables[i];
    }
    variables.clear();
    minijson::istream_context context(stream);
    type.Deserialize(context);
}
