//
//  Cloner.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Cloner.hpp"

using namespace Pocket;

void Cloner::TryStoreVariables() {
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
