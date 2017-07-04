//
//  testFieldDataCollection.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 01/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TypeInfo.hpp"
#include "Vector2.hpp"
#include "minijson_writer.hpp"
#include <iostream>

using namespace Pocket;

struct Component {
    Vector2 position;
    Vector2 rotation;
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(position)
    TYPE_FIELD(rotation)
    TYPE_FIELDS_END
};

int main_fieldDataCollection() {
    
    Component transform;
    auto type = transform.GetType();
    auto field = type.GetField("position");
    auto collection = field->CreateDataCollection();
    
    transform.position = {1,2};
    collection->AddData(0.0f, field.get());
    
    transform.position = {3,4};
    collection->AddData(1.0f, field.get());

    for (int i=0; i<100; i++) {
    
        transform.position = {5+(float)i,6-(float)i};
        collection->AddData(i/100.0f, field.get());
    }
    auto collectionData = collection->GetType();
    
    
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(std::cout, config);
    collectionData.Serialize(writer);
    writer.close();


    return 0;
}
