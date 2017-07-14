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
#include <memory>
#include "MetaLibrary.hpp"
#include "JsonSerializer.hpp"
#include "TypeIndexList.hpp"
#include "IFieldEditor.hpp"
#include "Timeline.hpp"
#include "TypeInterpolator.hpp"

namespace Pocket {

template<class T>
class FieldInfo;

class TypeInfo;

class FieldInfoAny;

class IFieldInfoTimeline;

template<class T>
class FieldInfoTimeline;

template<class T, typename S = void>
struct TimelineCreator {
    static std::unique_ptr<IFieldInfoTimeline> CreateTimeline() {
        return std::make_unique<FieldInfoTimeline<T>>();
    }
};

template<typename T>
struct TimelineCreator<std::unique_ptr<T>> {
    static std::unique_ptr<IFieldInfoTimeline> CreateTimeline() {
        return nullptr;
    }
};

template<typename T>
struct TimelineCreator<std::vector<std::unique_ptr<T>>> {
    static std::unique_ptr<IFieldInfoTimeline> CreateTimeline() {
        return nullptr;
    }
};

template<class T, typename S = void>
struct FieldInfoCopier {
    static void Copy(IFieldInfo* source, IFieldInfo* dest);
};

template<typename T>
struct FieldInfoCopier<std::unique_ptr<T>> {
    static void Copy(IFieldInfo* source, IFieldInfo* dest) {}
};

template<typename T>
struct FieldInfoCopier<std::vector<std::unique_ptr<T>>> {
    static void Copy(IFieldInfo* source, IFieldInfo* dest) {}
};

class IFieldInfo {
public:
    virtual ~IFieldInfo() { }
    std::string name;
    int type;
    virtual void Serialize(minijson::object_writer& writer) = 0;
    virtual void Deserialize(minijson::istream_context& context, minijson::value& value) = 0;
    virtual std::unique_ptr<IFieldInfo> Clone() = 0;
    virtual void SetFromAny(FieldInfoAny* any) = 0;
    virtual IFieldEditor* CreateEditor() = 0;
    virtual void SetFromOther(IFieldInfo* other) = 0;
    virtual std::unique_ptr<IFieldInfoTimeline> CreateTimeline() = 0;
};

template<class T>
class FieldInfo : public IFieldInfo {
public:
    
    void Serialize(minijson::object_writer& writer) override {
        JsonSerializer<T>::Serialize(name, *field, writer);
    }
    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        JsonSerializer<T>::Deserialize(value, field, context);
    }
    
    std::unique_ptr<IFieldInfo> Clone() override {
        auto clone = std::make_unique<FieldInfo<T>>();
        clone->name = this->name;
        clone->type = this->type;
        clone->field = this->field;
        return std::unique_ptr<IFieldInfo>( std::move( clone ) );
    }
    
    void SetFromAny(FieldInfoAny* any) override;
    
    IFieldEditor* CreateEditor() override {
        IFieldEditor* editor = FieldEditorCreator<T>::Create(field);
        return editor;
    }
    
    void SetFromOther(IFieldInfo* other) override {
        FieldInfoCopier<T>::Copy(other, this);
    }
    
    std::unique_ptr<IFieldInfoTimeline> CreateTimeline() override {
        return TimelineCreator<T>::CreateTimeline();
    }

    friend class TypeInfo;
public:
    T* field;
};

template<class T, typename S>
void FieldInfoCopier<T,S>::Copy(IFieldInfo* source, IFieldInfo* dest) {
    FieldInfo<T>* sourceTyped = static_cast<FieldInfo<T>*>(source);
    FieldInfo<T>* destTyped = static_cast<FieldInfo<T>*>(dest);
    (*destTyped->field) = (*sourceTyped->field);
}

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
        auto serializedField = std::make_unique<FieldInfo<T>>();
        serializedField->name = name;
        serializedField->field = &field;
        serializedField->type = 0;
        fields.emplace_back(std::move( serializedField ) );
    }
    
    void Serialize(minijson::object_writer& writer) {
        for(auto& field : fields) {
            field->Serialize(writer);
        }
    }
    
    template<class Context>
    void Deserialize(Context& context) {
        try {
            minijson::parse_object(context, [&] (const char* name, minijson::value v) {
                auto field = GetFieldInternal(name);
                if (field) {
                    field->Deserialize(context, v);
                } else {
                    minijson::ignore(context);
                }
            });
        } catch (minijson::parse_error e) {
            std::cout<< e.what() << std::endl;
        }
    }
    
    std::unique_ptr<IFieldInfo> GetField(const std::string& name) {
        for(auto& field : fields) {
            if (field->name == name) return field->Clone();
        }
        return nullptr;
    }

    using Fields = std::vector<std::unique_ptr<IFieldInfo>>;
    Fields fields;
    std::string name;
    
    void UpdateFromPointer(TypeInfo* info, TypeIndexList& list) {
        name = info->name;
        for(auto& field : info->fields) {
            list.AddToTypeInfo(this, field->type, field.get());
        }
    }

public:
    IFieldInfo* GetFieldInternal(const std::string& name) {
        for(auto& field : fields) {
            if (field->name == name) return field.get();
        }
        return nullptr;
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
    
    struct Object {
        
        void Set(const minijson::value& value) {
            valueType = value.type();
            string_value = value.as_string();
            long_value = value.as_long();
            double_value = value.as_double();
        }
        
        minijson::value_type valueType;
        std::string string_value;
        long long_value;
        double double_value;
            
        std::vector<Object> children;
    };
    
    Object root;

    void Serialize(minijson::object_writer& writer) override {
        SerializeInternal(writer, root);
    }
    
    void Serialize(minijson::array_writer& writer) {
        SerializeInternal(writer, root, false);
    }
    
    void SerializeInternal(minijson::object_writer& writer, Object& root) {
        switch (root.valueType) {
            case minijson::String: {
                JsonSerializer<std::string>::Serialize(name, root.string_value, writer);
                break;
            }
            case minijson::Number: {
                JsonSerializer<double>::Serialize(name, root.double_value, writer);
                break;
            }
            case minijson::Boolean: {
                JsonSerializer<bool>::Serialize(name, root.long_value == 0 ? false : true, writer);
                break;
            }
            case minijson::Object:
            case minijson::Array: {
                minijson::array_writer array = writer.nested_array(root.string_value.c_str());
                for(auto& child : root.children) {
                    SerializeInternal(array, child, true);
                }
                array.close();
                break;
            }
            case minijson::Null: {
                break;
            }
        }
    }

    void SerializeInternal(minijson::array_writer& writer, Object& root, bool createArrayObject) {
        switch (root.valueType) {
            case minijson::String: {
                JsonSerializer<std::string>::Serialize(root.string_value, writer);
                break;
            }
            case minijson::Number: {
                JsonSerializer<double>::Serialize(root.double_value, writer);
                break;
            }
            case minijson::Boolean: {
                JsonSerializer<bool>::Serialize(root.long_value == 0 ? false : true, writer);
                break;
            }
            case minijson::Object:
            case minijson::Array: {
                if (createArrayObject) {
                    minijson::array_writer array = writer.nested_array();
                    for(auto& child : root.children) {
                        SerializeInternal(array, child, true);
                    }
                    array.close();
                } else {
                    for(auto& child : root.children) {
                        SerializeInternal(writer, child, true);
                    }
                }
                break;
            }
            case minijson::Null: {
                break;
            }
        }
    }

    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        DeserializeInternal(context, value, root);
    }
    
    void DeserializeInternal(minijson::istream_context& context, minijson::value& value, Object& parent) {
        parent.Set(value);
        if (value.type()==minijson::Array) {
            minijson::parse_array(context, [&] (minijson::value v) {
                parent.children.resize(parent.children.size() + 1);
                DeserializeInternal(context, v, parent.children.back());
            });
        }
    }
    
    std::unique_ptr<IFieldInfo> Clone() override {
        auto clone = std::make_unique<FieldInfoAny>();
        clone->name = name;
        clone->root = root;
        return std::unique_ptr<IFieldInfo>(std::move(clone));
    }
    
    IFieldEditor* CreateEditor() override {
        return 0;
    }
    
    void SetFromAny(FieldInfoAny* any) override {}
    void SetFromOther(IFieldInfo* other) override {}
    
    std::unique_ptr<IFieldInfoTimeline> CreateTimeline() override {
        return nullptr;
    }

};


template<typename T>
void FieldInfo<T>::SetFromAny(FieldInfoAny* any) {
    std::stringstream s;
    minijson::writer_configuration config;
    if (any->root.valueType == minijson::Array) {
        minijson::array_writer writer(s, config);
        any->Serialize(writer);
        writer.close();
    } else {
        minijson::object_writer writer(s, config);
        any->Serialize(writer);
        writer.close();
    }
   
    
    std::cout << s.str() << std::endl;
    minijson::istream_context context(s);
    
    minijson::value val(any->root.valueType, any->root.string_value.c_str(), any->root.long_value, any->root.double_value);
    
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

template<typename T>
struct JsonSerializer<std::shared_ptr<T>> {
    static void Serialize(std::string& key, const std::shared_ptr<T>& value, minijson::object_writer& writer) {
        JsonSerializer<T*>::Serialize(key, value.get(), writer);
    }
    
    static void Serialize(const std::shared_ptr<T>& value, minijson::array_writer& writer) {
        JsonSerializer<T*>::Serialize(value.get(), writer);
    }

    static void Deserialize(minijson::value& value, std::shared_ptr<T>* field, minijson::istream_context& context) {
        T* data;
        JsonSerializer<T*>::Deserialize(value, &data, context);
        field->reset(data);
    }
};

template<typename T>
struct JsonSerializer<std::unique_ptr<T>> {
    static void Serialize(std::string& key, const std::unique_ptr<T>& value, minijson::object_writer& writer) {
        JsonSerializer<T*>::Serialize(key, value.get(), writer);
    }
    
    static void Serialize(const std::unique_ptr<T>& value, minijson::array_writer& writer) {
        JsonSerializer<T*>::Serialize(value.get(), writer);
    }

    static void Deserialize(minijson::value& value, std::unique_ptr<T>* field, minijson::istream_context& context) {
        T* data;
        JsonSerializer<T*>::Deserialize(value, &data, context);
        field->reset(data);
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
        for(auto& field : type.fields) {
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
        for(auto& field : type.fields) {
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

template<typename T>
struct FieldEditorCreator<std::shared_ptr<T>> {
    static IFieldEditor* Create(std::shared_ptr<T>* ptr) {
        T* p = ptr->get();
        return FieldEditorCreator<T*>::Create(&p);
    }
};

template<typename T>
struct FieldEditorCreator<std::unique_ptr<T>> {
    static IFieldEditor* Create(std::unique_ptr<T>* ptr) {
        T* p = ptr->get();
        return FieldEditorCreator<T*>::Create(&p);
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

template<typename T>
struct FieldInfoTimelineData {
    Timeline<T> t;
};

template<class T>
class FieldInfoTimeline;

class IFieldInfoTimeline {
public:
    virtual ~IFieldInfoTimeline() { }
    virtual void AddData(float time, IFieldInfo* field) = 0;
    virtual void Apply(float time, IFieldInfo* field) = 0;
    virtual TypeInfo GetType() = 0;
    virtual std::unique_ptr<IFieldInfoTimeline> Clone() = 0;
    
    CONSTRUCTOR_BASE(IFieldInfoTimeline)
};

template<class T>
class FieldInfoTimeline : public IFieldInfoTimeline {
public:
    void AddData(float time, IFieldInfo* field) override {
        FieldInfo<T>* f = static_cast<FieldInfo<T>*>(field);
        timeline.t.AddNode(time, *f->field);
    }
    
    void Apply(float time, IFieldInfo* field) override {
        FieldInfo<T>* f = static_cast<FieldInfo<T>*>(field);
        timeline.t.Evaluate(time, [f] (float time, const T& a, const T& b) {
            TypeInterpolator<T>::Interpolate(f->field, time, a, b);
        });
    }
    
    TypeInfo GetType() override {
        TypeInfo info;
        info.AddField(timeline, "timeline");
        return info;
    }

    std::unique_ptr<IFieldInfoTimeline> Clone() override {
        std::unique_ptr<FieldInfoTimeline<T>> clone = std::make_unique<FieldInfoTimeline<T>>();
        clone->timeline = this->timeline;
        return std::unique_ptr<IFieldInfoTimeline>(std::move(clone));
    }
    
    
public:
    FieldInfoTimelineData<T> timeline;

    CONSTRUCTOR_DERIVED(FieldInfoTimeline, T)
};

CONSTRUCTOR_DERIVED_INITIALIZER(FieldInfoTimeline)

template<typename S>
struct TimelineCreator<FieldInfoTimelineData<S>> {
    static std::unique_ptr<IFieldInfoTimeline> CreateTimeline() {
        return nullptr;
    }
};

template<typename S>
struct JsonSerializer<FieldInfoTimelineData<S>> {

    static void Serialize(std::string& key, const FieldInfoTimelineData<S>& value, minijson::object_writer& writer) {
        minijson::object_writer object = writer.nested_object(key.c_str());
        std::string keys = "keys";
        std::string values = "values";
        
        JsonSerializer<std::vector<float>>::Serialize(keys, value.t.keys, object);
        JsonSerializer<std::vector<S>>::Serialize(values, value.t.values, object);
        object.close();
    }
    
    static void Serialize(const FieldInfoTimelineData<S>& value, minijson::array_writer& writer) {
        JsonSerializer<std::vector<float>>::Serialize(value.t.keys, writer);
        JsonSerializer<std::vector<S>>::Serialize(value.t.values, writer);
    }
    
    static void Deserialize(minijson::value& value, FieldInfoTimelineData<S>* field, minijson::istream_context& context) {
        try {
            minijson::parse_object(context, [&] (const char* name, minijson::value v) {
                std::string n = std::string(name);
                if (n == "keys") {
                    JsonSerializer<std::vector<float>>::Deserialize(v, &field->t.keys, context);
                } else if (n == "values") {
                    JsonSerializer<std::vector<S>>::Deserialize(v, &field->t.values, context);
                }
            });
        } catch (std::exception e) {
            std::cout<< e.what() << std::endl;
        }
    }
};

}
