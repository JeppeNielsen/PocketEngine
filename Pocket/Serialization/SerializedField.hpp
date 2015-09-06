//
//  SerializedField.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 06/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <type_traits>
#include "JsonSerializer.hpp"

namespace Pocket {


  
template<class T>
class SerializedField;

class SerializedFieldCollection;



class ISerializedField {
public:
    virtual ~ISerializedField() { }
    std::string name;
    virtual void Serialize(minijson::object_writer& writer) = 0;
};

template<class T>
class SerializedField : public ISerializedField {
public:
    ~SerializedField() { }
    
    void Serialize(minijson::object_writer& writer) override {
        JsonSerializer<T>::Serialize(name, *field, writer);
    }
    friend class SerializedFieldCollection;
private:
    T* field;
};
  
class SerializedFieldCollection {
public:
    SerializedFieldCollection() { }
    
    ~SerializedFieldCollection() {
        for (size_t i=0; i<fields.size(); ++i) {
            delete fields[i];
        }
    }
    
    SerializedFieldCollection(SerializedFieldCollection&& other) {
        fields = other.fields;
        other.fields.clear();
    }
    
    template<class T>
    void AddField(T& field, std::string name) {
        SerializedField<T>* serializedField = new SerializedField<T>();
        serializedField->name = name;
        serializedField->field = &field;
        fields.push_back(serializedField);
    }
    
    void Serialize(minijson::object_writer& writer) {
        for(auto field : fields) {
            field->Serialize(writer);
        }
    }
    
private:
    typedef std::vector<ISerializedField*> Fields;
    Fields fields;
};

class ISerializable {
public:
    virtual SerializedFieldCollection GetFields() { SerializedFieldCollection fields; return fields; }
};

template<typename T>
struct JsonSerializer<T, typename std::enable_if< std::is_base_of<ISerializable, T>::value >::type> {
    static void Serialize(std::string& key, const ISerializable& value, minijson::object_writer& writer) {
        auto fields = ((ISerializable&)value).GetFields();
        minijson::object_writer object = writer.nested_object(key.c_str());
        fields.Serialize(object);
        object.close();
    }
    
    static void Serialize(const ISerializable& value, minijson::array_writer& writer) {
        auto fields = ((ISerializable&)value).GetFields();
        minijson::object_writer object = writer.nested_object();
        fields.Serialize(object);
        object.close();
    }
};
 
}

#define SERIALIZE_FIELDS_BEGIN \
SerializedFieldCollection GetFields() override { \
SerializedFieldCollection fields;

#define SERIALIZE_FIELD(field) \
fields.AddField(field, #field);

#define SERIALIZE_FIELDS_END \
return fields; \
}


