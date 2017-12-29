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
#include <memory>

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
    virtual std::unique_ptr<IFieldInfo> Clone() = 0;
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
    
    std::unique_ptr<IFieldInfo> Clone() override {
        FieldInfo<T>* clone = new FieldInfo<T>();
        clone->name = this->name;
        clone->type = this->type;
        clone->field = this->field;
        return std::unique_ptr<IFieldInfo>( clone );
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
    
    TypeInfo() {}
    
    TypeInfo(TypeInfo&& other) {
        fields = std::move( other.fields );
        name = other.name;
        other.fields.clear();
        other.name.clear();
    }
    
    TypeInfo(const TypeInfo& other) {
        fields.clear();
        for(auto& field : other.fields) {
            fields.push_back(field->Clone());
        }
        name = other.name;
    }
    
    void operator = (const TypeInfo& other) {
        fields.clear();
        for(auto& field : other.fields) {
            fields.push_back(field->Clone());
        }
        name = other.name;
    }

    
    template<class T>
    void AddField(T& field, std::string name) {
    }
    
    template<class T>
    void AddScriptField(T& field, std::string name) {
        auto serializedField = new FieldInfo<T>();
        serializedField->name = name;
        serializedField->field = &field;
        serializedField->type = FieldInfoIndexer<T>::Index();
        fields.emplace_back(std::unique_ptr<IFieldInfo>(serializedField) );
    }

    using Fields = std::vector<std::unique_ptr<IFieldInfo>>;
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
