//
//  FieldInfo.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 06/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <iostream>
#include <type_traits>
#include "MetaLibrary.hpp"
#include "JsonSerializer.hpp"

namespace Pocket {

template<class T>
class FieldInfo;

class TypeInfo;

struct IFieldInfoEditor {
    virtual ~IFieldInfoEditor() { }
    virtual void Create(void* field, void* context, void* parent) { }
    virtual void Destroy() { }
    virtual void Update(float dt) { }
};

template<typename Field, typename Context, typename Parent>
struct FieldInfoEditor : public IFieldInfoEditor {
    void Create(void* field, void* context, void* parent) {
        this->field = (Field*)field;
        Initialize((Context*)context, (Parent*)parent);
    }
    virtual void Initialize(Context* context, Parent* parent) { }
    virtual void Update(float dt) { }
    Field* field;
};

template<typename Field>
struct FieldInfoEditorCreator {
    static IFieldInfoEditor* Create() { return 0; }
};


template<typename T>
struct FieldEditorProperty : public FieldInfoEditor<Property<T>, void, void> {
    IFieldInfoEditor* editor;
    
    FieldEditorProperty() : editor(0) {}
    ~FieldEditorProperty() { delete editor; }
    
    void Initialize(void* context, void* parent) {
        editor = FieldInfo<T>::Editor ? FieldInfo<T>::Editor() : 0;
        if (editor) {
            editor->Create(&currentValue, context, parent);
        }
    }
    
    void Destroy() {
        if (editor) {
            editor->Destroy();
        }
    }
    
    void Update(float dt) {
        if (!editor) return;
        
        if (currentValue!=prevValue) {
            prevValue = currentValue;
            this->field->operator=(currentValue);
        } else {
            currentValue = this->field->operator()();
            prevValue = currentValue;
        }
        editor->Update(dt);
    }
    
    T currentValue;
    T prevValue;
};


template<typename T>
struct FieldInfoEditorCreator<Property<T>> {
    static IFieldInfoEditor* Create() {
        IFieldInfoEditor* editor = FieldInfo<T>::Editor ? FieldInfo<T>::Editor() : 0;
        if (!editor) {
            return 0;
        } else {
            delete editor;
        }
        return new FieldEditorProperty<T>();
    }
};

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
        JsonSerializer<T>::Serialize(name, *field, writer);
    }
    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        JsonSerializer<T>::Deserialize(value, field, context);
    }
    
    IFieldInfoEditor* CreateEditor(void* context, void* parent) {
        IFieldInfoEditor* editor = Editor ? Editor() : 0;
        if (!editor) {
            editor = FieldInfoEditorCreator<T>::Create();
        }
        if (!editor) return 0;
        editor->Create(field, context, parent);
        return editor;
    }
    
    bool HasEditor() {
        if (Editor) return true;
        auto editor = FieldInfoEditorCreator<T>::Create();
        if (editor) {
            delete editor;
            return true;
        } else {
            return false;
        }
    }
    
    static std::function<IFieldInfoEditor*()> Editor;
    
    friend class TypeInfo;
public:
    T* field;
};



template<class T>
std::function<IFieldInfoEditor*()> FieldInfo<T>::Editor = 0;

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
        serializedField->type = 0;
        fields.push_back(serializedField);
    }
    
    void Serialize(minijson::object_writer& writer) {
        for(auto field : fields) {
            field->Serialize(writer);
        }
    }
    
    template<class Context>
    void Deserialize(Context& context) {
        try {
            minijson::parse_object(context, [&] (const char* name, minijson::value v) {
                auto field = GetField(name);
                if (field) {
                    field->Deserialize(context, v);
                } else if (v.type() == minijson::Object){
                    minijson::ignore(context);
                }
            });
        } catch (std::exception e) {
            std::cout<< e.what() << std::endl;
        }
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
    
    
    void UpdateFromPointer(TypeInfo* info) {
        for(auto field : info->fields) {
            AddField(field);
        }
    }
    
    void AddField(IFieldInfo* fieldInfo) {
        switch (fieldInfo->type) {
            case 0: AddField(*static_cast<FieldInfo<int>*>(fieldInfo)->field, fieldInfo->name); break;
            case 1: AddField(*static_cast<FieldInfo<float>*>(fieldInfo)->field, fieldInfo->name); break;
            case 2: AddField(*static_cast<FieldInfo<double>*>(fieldInfo)->field, fieldInfo->name); break;
            case 3: AddField(*static_cast<FieldInfo<std::string>*>(fieldInfo)->field, fieldInfo->name); break;
        }
    }
};

template<typename T>
struct JsonSerializer<T, typename std::enable_if_t< Pocket::Meta::HasGetTypeFunction::apply<T>::value >> {
    static void Serialize(std::string& key, const T& value, minijson::object_writer& writer) {
        auto type = ((T&)value).GetType();
        minijson::object_writer object = writer.nested_object(key.c_str());
        type.Serialize(object);
        object.close();
    }
    
    static void Serialize(const T& value, minijson::array_writer& writer) {
        auto type = ((T&)value).GetType();
        minijson::object_writer object = writer.nested_object();
        type.Serialize(object);
        object.close();
    }
    
    static void Deserialize(minijson::value& value, T* object, minijson::istream_context& context) {
        auto type = object->GetType();
        type.Deserialize(context);
    }
};

inline std::string className(const std::string& prettyFunction)
{
    size_t colons = prettyFunction.find("::");
    if (colons == std::string::npos)
        return "::";
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;

    return prettyFunction.substr(begin,end);
}

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)

#define TYPE_FIELDS_BEGIN \
public: \
TypeInfo GetType() { \
TypeInfo fields; \
fields.name = __CLASS_NAME__;

#define TYPE_FIELD(field) \
fields.AddField(field, #field);

#define TYPE_FIELDS_END \
return fields; \
} \
private:

}