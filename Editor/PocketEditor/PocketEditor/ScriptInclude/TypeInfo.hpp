//
//  TypeInfo.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include "FieldInfoIndexer.hpp"
#include <functional>

namespace Pocket {
  
template<class T>
class FieldInfo;

class TypeInfo;
class IFieldInfoEditor;

namespace minijson {
    class object_writer;
    class istream_context;
    class value;
}

class IFieldInfo {
public:
    virtual ~IFieldInfo() { }
    std::string name;
    int type;
    virtual void Serialize(minijson::object_writer& writer) = 0;
    virtual void Deserialize(minijson::istream_context& context, minijson::value& value) = 0;
    virtual IFieldInfoEditor* CreateEditor(void* context, void* parent) = 0;
    virtual bool HasEditor() = 0;
};

template<class T>
class FieldInfo : public IFieldInfo {
public:
    ~FieldInfo() { }
    
    void Serialize(minijson::object_writer& writer) override {
    }
    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        
    }
    
    IFieldInfoEditor* CreateEditor(void* context, void* parent) override {
       return 0;
    }
    
    bool HasEditor() override {
        return false;
    }
    
    static std::function<IFieldInfoEditor*()> Editor;
    
    friend class TypeInfo;
public:
    T* field;
};



class TypeInfo {
public:
    TypeInfo() { }
    
    ~TypeInfo() {
        for (size_t i=0; i<fields.size(); ++i) {
            delete fields[i];
        }
    }
    
    TypeInfo(TypeInfo&& other) {
        fields = other.fields;
        other.fields.clear();
    }
    
    template<class T>
    void AddField(T& field, std::string name) {
        FieldInfo<T>* serializedField = new FieldInfo<T>();
        serializedField->name = name;
        serializedField->field = &field;
        serializedField->type = FieldInfoIndexer<T>::Index();
        fields.push_back(serializedField);
    }
    
    IFieldInfo* GetField(std::string name) {
        for(auto field : fields) {
            if (field->name == name) return field;
        }
        return 0;
    }

    using Fields = std::vector<IFieldInfo*>;
    Fields fields;
    std::string name;
};

#define TYPE_FIELDS_BEGIN
#define TYPE_FIELD(field)
#define TYPE_FIELDS_END

}