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

struct ISerializedFieldEditor {
    virtual ~ISerializedFieldEditor() { }
    virtual void Create(void* field, void* context, void* parent) { }
    virtual void Destroy() { }
    virtual void Update(float dt) { }
};

template<typename Field, typename Context, typename Parent>
struct SerializedFieldEditor : public ISerializedFieldEditor {
    void Create(void* field, void* context, void* parent) {
        this->field = (Field*)field;
        Initialize((Context*)context, (Parent*)parent);
    }
    virtual void Initialize(Context* context, Parent* parent) { }
    virtual void Update(float dt) { }
    Field* field;
};

template<typename Field>
struct SerializedFieldEditorCreator {
    static ISerializedFieldEditor* Create() { return 0; }
};


template<typename Owner, typename T>
struct FieldEditorProperty : public SerializedFieldEditor<Property<Owner, T>, void, void> {
    ISerializedFieldEditor* editor;
    
    FieldEditorProperty() : editor(0) {}
    ~FieldEditorProperty() { delete editor; }
    
    void Initialize(void* context, void* parent) {
        editor = SerializedField<T>::Editor ? SerializedField<T>::Editor() : 0;
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
            this->field->Set(currentValue);
        } else {
            currentValue = this->field->GetValue();
            prevValue = currentValue;
        }
        editor->Update(dt);
    }
    
    T currentValue;
    T prevValue;
};


template<typename Owner, typename T>
struct SerializedFieldEditorCreator<Property<Owner, T>> {
    static ISerializedFieldEditor* Create() {
        ISerializedFieldEditor* editor = SerializedField<T>::Editor ? SerializedField<T>::Editor() : 0;
        if (!editor) {
            return 0;
        } else {
            delete editor;
        }
        return new FieldEditorProperty<Owner, T>();
    }
};

class ISerializedField {
public:
    virtual ~ISerializedField() { }
    std::string name;
    virtual void Serialize(minijson::object_writer& writer) = 0;
    virtual void Deserialize(minijson::istream_context& context, minijson::value& value) = 0;
    virtual ISerializedFieldEditor* CreateEditor(void* context, void* parent) = 0;
    virtual bool HasEditor() = 0;
};

template<class T>
class SerializedField : public ISerializedField {
public:
    ~SerializedField() { }
    
    void Serialize(minijson::object_writer& writer) override {
        JsonSerializer<T>::Serialize(name, *field, writer);
    }
    
    void Deserialize(minijson::istream_context& context, minijson::value& value) override {
        JsonSerializer<T>::Deserialize(value, field, context);
    }
    
    ISerializedFieldEditor* CreateEditor(void* context, void* parent) {
        ISerializedFieldEditor* editor = Editor ? Editor() : 0;
        if (!editor) {
            editor = SerializedFieldEditorCreator<T>::Create();
        }
        if (!editor) return 0;
        editor->Create(field, context, parent);
        return editor;
    }
    
    bool HasEditor() {
        if (Editor) return true;
        auto editor = SerializedFieldEditorCreator<T>::Create();
        if (editor) {
            delete editor;
            return true;
        } else {
            return false;
        }
    }
    
    static std::function<ISerializedFieldEditor*()> Editor;
    
    friend class SerializedFieldCollection;
public:
    T* field;
};

template<class T>
std::function<ISerializedFieldEditor*()> SerializedField<T>::Editor = 0;
  
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
    
public:

    ISerializedField* GetField(std::string name) {
        for(auto field : fields) {
            if (field->name == name) return field;
        }
        return 0;
    }

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
    
    static void Deserialize(minijson::value& value, ISerializable* field, minijson::istream_context& context) {
        auto fields = field->GetFields();
        fields.Deserialize(context);
    }
};


 
}

#define SERIALIZE_FIELDS_BEGIN \
public: \
SerializedFieldCollection GetFields() override { \
SerializedFieldCollection fields;

#define SERIALIZE_FIELD(field) \
fields.AddField(field, #field);

#define SERIALIZE_FIELDS_END \
return fields; \
} \
private:


