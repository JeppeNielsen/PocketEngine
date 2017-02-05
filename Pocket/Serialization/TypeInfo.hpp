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
#include "TypeIndexList.hpp"
#include "IFieldEditor.hpp"

namespace Pocket {

template<class T>
class FieldInfo;

class TypeInfo;



/*
template<typename T>
struct FieldEditorProperty : public FieldInfoEditor<Property<T>, void, void> {
    IFieldEditor* editor;
    
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
*/

class FieldInfoAny;

class IFieldInfo {
public:
    virtual ~IFieldInfo() { }
    std::string name;
    int type;
    virtual void Serialize(minijson::object_writer& writer) = 0;
    virtual void Deserialize(minijson::istream_context& context, minijson::value& value) = 0;
    virtual IFieldInfo* Clone() = 0;
    virtual void SetFromAny(FieldInfoAny* any) = 0;
    virtual IFieldEditor* CreateEditor() = 0;
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
    
    IFieldInfo* Clone() override {
        FieldInfo<T>* clone = new FieldInfo<T>();
        clone->name = this->name;
        clone->type = this->type;
        clone->field = this->field;
        return clone;
    }
    
    void SetFromAny(FieldInfoAny* any) override;
    
    IFieldEditor* CreateEditor() override {
        IFieldEditor* editor = FieldEditorCreator<T>::Create(field);
        return editor;
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
        name = other.name;
        other.fields.clear();
        other.name.clear();
    }
    
    TypeInfo(const TypeInfo& other) {
        fields.clear();
        for(auto f : other.fields) {
            fields.push_back(f->Clone());
        }
        name = other.name;
    }
    
    void operator = (const TypeInfo& other) {
        fields.clear();
        for(auto f : other.fields) {
            fields.push_back(f->Clone());
        }
        name = other.name;
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
    
    
    void UpdateFromPointer(TypeInfo* info, TypeIndexList& list) {
        name = info->name;
        for(auto field : info->fields) {
            list.AddToTypeInfo(this, field->type, field);
        }
    }
};

template<typename T>
void TypeIndexList::TypeIndex<T>::AddToTypeInfo(Pocket::TypeInfo *typeInfo, Pocket::IFieldInfo* fieldInfo) {
    FieldInfo<T>* derived = static_cast<FieldInfo<T>*>(fieldInfo);
    typeInfo->AddField<T>(*derived->field, fieldInfo->name);
}



template<typename T>
struct TypeIndexList::TypeIndex<std::vector<T>> : public TypeIndexList::ITypeIndex {
    
    void AddToTypeInfo(Pocket::TypeInfo *typeInfo, Pocket::IFieldInfo* fieldInfo) override {
        FieldInfo<std::vector<T>>* derived = static_cast<FieldInfo<std::vector<T>>*>(fieldInfo);
        typeInfo->AddField<std::vector<T>>(*derived->field, fieldInfo->name);
    }

    std::string GetName() override {
        return "std::vector<" + TypeIndexList::GetClassName<T>()+">";
    };
};


template<typename T>
struct JsonSerializer<T, typename std::enable_if< Pocket::Meta::HasGetTypeFunction::apply<T>::value >::type> {
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

class FieldInfoAny : public IFieldInfo {
public:
    FieldInfoAny() { type = -1; }

    void Serialize(minijson::object_writer& writer) override {
        switch (valueType) {
            case minijson::String: {
                JsonSerializer<std::string>::Serialize(name, string_value, writer);
                break;
            }
            case minijson::Number: {
                JsonSerializer<double>::Serialize(name, double_value, writer);
                break;
            }
            case minijson::Boolean: {
                JsonSerializer<bool>::Serialize(name, long_value == 0 ? false : true, writer);
                break;
            }
            case minijson::Object:
            case minijson::Array: {
                break;
            }
            case minijson::Null: {
                break;
            }
        }
    }
    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        valueType = value.type();
        string_value = value.as_string();
        long_value = value.as_long();
        double_value = value.as_double();
    }
    
    IFieldInfo* Clone() override {
        FieldInfoAny* clone = new FieldInfoAny();
        clone->name = name;
        clone->valueType = valueType;
        clone->string_value = string_value;
        clone->long_value = long_value;
        clone->double_value = double_value;
        return clone;
    }
    
    IFieldEditor* CreateEditor() override {
        return 0;
    }
    
    void SetFromAny(FieldInfoAny* any) override {}
    
    minijson::value_type valueType;
    std::string string_value;
    long long_value;
    double double_value;
};


template<typename T>
void FieldInfo<T>::SetFromAny(FieldInfoAny* any) {
    std::stringstream s;
    s<<any->string_value;
    minijson::istream_context context(s);
    minijson::value val(any->valueType, any->string_value.c_str(), any->long_value, any->double_value);
    JsonSerializer<T>::Deserialize(val, field, context);
    //(*field) = T::Deserialize(any->value);
    //std::cout << any->value << std::endl;
}


template<>
struct JsonSerializer<IFieldInfo*> {
    static void Serialize(std::string& key, const IFieldInfo* value, minijson::object_writer& writer) {
        //writer.write(key.c_str(), value);
        
        IFieldInfo* info = (IFieldInfo*)value;
        info->Serialize(writer);
    }
    
    static void Serialize(const IFieldInfo* value, minijson::array_writer& writer) {
        
        minijson::object_writer object = writer.nested_object();
        IFieldInfo* info = (IFieldInfo*)value;
        info->Serialize(object);
        object.close();
    }
    
    static void Deserialize(minijson::value& value, IFieldInfo** field, minijson::istream_context& context) {
        //if (value.type() != minijson::Object) return;
        //(*field) = (int)value.as_long();
        
        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            (*field) = new FieldInfoAny();
            (*field)->name = std::string(n);
            (*field)->Deserialize(context, v);
        });
    }
};

struct TypeEditorTitle {
    using Callback = std::function<void*(void* context, void* parent, const std::string& title)>;
    static Callback Title;
    
    template<typename Class>
    static std::string GetClassName() {
#ifdef WIN32
        std::string functionName = __FUNCTION__;
#else
        std::string functionName = __PRETTY_FUNCTION__;
#endif
        const std::string token = "Class = ";
        size_t equal = functionName.find(token) + token.size();
        return functionName.substr(equal, functionName.size() - equal - 1);
    }
};

//TypeEditorTitle::Callback TypeEditorTitle::Title = 0;

template<typename T>
struct TypeEditor : public IFieldEditor {
    
    std::vector<IFieldEditor*> fieldEditors;
    
    void SetField(void* field) override {
        this->field = static_cast<T*>(field);
        type = this->field->GetType();
    }

    void Create(void* context, void* parent) override {
        for(auto field : type.fields) {
            auto editor = field->CreateEditor();
            if (!editor) continue;
            editor->Create(context, parent);
            fieldEditors.push_back(editor);
        }
        if (TypeEditorTitle::Title) {
            TypeEditorTitle::Title(context, parent, TypeEditorTitle::GetClassName<T>());
        }
    }
    
    void Update(float dt) override {
       for(auto editor : fieldEditors) {
            editor->Update(dt);
       }
    }
    
    void Destroy() override {
       for(auto editor : fieldEditors) {
            editor->Destroy();
       }
    }
    
    T* field;
    TypeInfo type;
};

struct TypeInfoEditor : public IFieldEditor {
    
    std::vector<IFieldEditor*> fieldEditors;
    
    void SetField(void* field) override {
        TypeInfo* typeInfo = static_cast<TypeInfo*>(field);
        type = TypeInfo(*typeInfo);
    }

    void Create(void* context, void* parent) override {
        for(auto field : type.fields) {
            auto editor = field->CreateEditor();
            if (!editor) continue;
            editor->Create(context, parent);
            fieldEditors.push_back(editor);
        }
        if (TypeEditorTitle::Title) {
            TypeEditorTitle::Title(context, parent, type.name);
        }
    }
    
    void Update(float dt) override {
       for(auto editor : fieldEditors) {
            editor->Update(dt);
       }
    }
    
    void Destroy() override {
       for(auto editor : fieldEditors) {
            editor->Destroy();
       }
    }
    
    TypeInfo type;
};



template<typename T>
struct FieldEditorCreator<T, typename std::enable_if< Pocket::Meta::HasGetTypeFunction::apply<T>::value >::type> {
    static IFieldEditor* Create(T* ptr) {
        TypeEditor<T>* editor = new TypeEditor<T>();
        editor->SetField(ptr);
        return editor;
    }
};

template<>
struct FieldEditorCreator<IFieldInfo*> {
    static IFieldEditor* Create(IFieldInfo** ptr) {
        return (*ptr)->CreateEditor();
    }
};

struct EmptyComponentEditor : public IFieldEditor {
    
    void SetField(void* field) override {
        TypeInfo* typeInfo = static_cast<TypeInfo*>(field);
        name = typeInfo->name;
    }

    void Create(void* context, void* parent) override {
        if (TypeEditorTitle::Title) {
            TypeEditorTitle::Title(context, parent, name);
        }
    }
    
    void Update(float dt) override {}
    void Destroy() override { }
    
    std::string name;
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
#ifdef WIN32
#define __CLASS_NAME__ className(__FUNCTION__)
#else
#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)
#endif
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