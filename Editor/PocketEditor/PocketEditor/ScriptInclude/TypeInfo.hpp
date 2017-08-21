//
//  TypeInfo.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include <functional>

namespace Pocket {
  
template<class T>
class FieldInfo;

class TypeInfo;

template<typename T>
struct FieldInfoIndexer { static int Index() { return -1; } };

namespace minijson {
    class object_writer;
    class istream_context;
    class value;
}

struct IFieldEditor {
    virtual ~IFieldEditor() { }
    virtual void SetField(void* field) = 0;
    virtual void Create(void* context, void* parent) = 0;
    virtual void Destroy() = 0;
    virtual void Update(float dt) = 0;
};

template<class T, typename S = void>
struct FieldEditorCreator {
    static IFieldEditor* Create() {
        return 0;
    }
};

struct FieldInfoAny;

class IFieldInfo {
public:
    virtual ~IFieldInfo() { }
    std::string name;
    int type;
    virtual void Serialize(minijson::object_writer& writer) = 0;
    virtual void Deserialize(minijson::istream_context& context, minijson::value& value) = 0;
    virtual IFieldInfo* Clone() = 0;
    virtual void SetFromAny(FieldInfoAny* any) { }
    virtual IFieldEditor* CreateEditor() = 0;
};

template<class T>
class FieldInfo : public IFieldInfo {
public:
    ~FieldInfo() { }
    
    void Serialize(minijson::object_writer& writer) override {
        
    }
    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        
    }
    
    IFieldInfo* Clone() override {
        FieldInfo<T>* clone = new FieldInfo<T>();
        clone->name = this->name;
        clone->type = this->type;
        clone->field = this->field;
        return clone;
    }
    
    void SetFromAny(FieldInfoAny* any) override {
    
    }
    
    IFieldEditor* CreateEditor() override {
        return 0;
    }
    
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

#define CONSTRUCTOR_BASE(Type) 

#define CONSTRUCTOR_DERIVED(TypeName, TemplateName)

#define CONSTRUCTOR_DERIVED_INITIALIZER(TypeName)

}
