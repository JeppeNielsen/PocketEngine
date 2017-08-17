#pragma once
#include "Property.hpp"
#include <vector>

namespace Pocket {

struct IFieldEditor {
    virtual ~IFieldEditor() { }
    virtual void SetField(void* field) = 0;
    virtual void Create(const std::string& name, void* context, void* parent) = 0;
    virtual void Destroy() = 0;
    virtual void Update(float dt) = 0;
};

template<class T, typename S = void>
struct FieldEditorCreator {
    static IFieldEditor* Create(T* ptr) {
        return 0;
    }
};

template<typename T>
struct PropertyEditor : public IFieldEditor {

    Property<T>* field;
    IFieldEditor* editor;
    
    PropertyEditor() : editor(0) {}
    
    ~PropertyEditor() {
        if (editor) {
            delete editor;
        }
    }
    
    void SetField(void* field) override {
        this->field = static_cast<Property<T>*>(field);
    }

    void Create(const std::string& name, void* context, void* parent) override {
        currentValue = this->field->operator()();
        editor = FieldEditorCreator<T>::Create(&currentValue);
        if (editor) {
            editor->Create(name, context, parent);
        }
    }
    
    void Update(float dt) override {
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
    
    void Destroy() override {
        if (editor) {
            editor->Destroy();
        }
    }
    
    T currentValue;
    T prevValue;
};

template<typename T>
struct FieldEditorCreator<Property<T>> {
    static IFieldEditor* Create(Property<T>* ptr) {
        PropertyEditor<T>* editor = new PropertyEditor<T>();
        editor->SetField(ptr);
        return editor;
    }
};

template<typename T>
struct VectorEditor : public IFieldEditor {

    std::vector<IFieldEditor*> entries;
    
    ~VectorEditor() {
        for(auto entry : entries) {
            delete entry;
        }
    }
    
    void SetField(void* field) override {
        this->field = static_cast<std::vector<T>*>(field);
    }

    void Create(const std::string& name, void* context, void* parent) override {
        for(auto&& item : *this->field) {
            auto editor = FieldEditorCreator<T>::Create(&item);
            if (!editor) continue;
            editor->Create("Item", context, parent);
            entries.push_back(editor);
        }
    }
    
    void Update(float dt) override {
        for(auto entry : entries) {
            if (!entry) continue;
            entry->Update(dt);
        }
    }
    
    void Destroy() override {
        for(auto entry : entries) {
            if (!entry) continue;
            entry->Destroy();
        }
    }
    
    std::vector<T>* field;
};

template<>
struct FieldEditorCreator<std::vector<bool>> {
    static IFieldEditor* Create(std::vector<bool>* ptr) {
        return 0;
    }
};

template<typename T>
struct FieldEditorCreator<std::vector<T>> {
    static IFieldEditor* Create(std::vector<T>* ptr) {
        VectorEditor<T>* editor = new VectorEditor<T>();
        editor->SetField(ptr);
        return editor;
    }
};


template<typename T>
struct EnumEditor : public IFieldEditor {

    IFieldEditor* editor;
    
    ~EnumEditor() {
        delete editor;
    }
    
    void SetField(void* field) override {
        this->field = static_cast<T*>(field);
    }

    void Create(const std::string& name, void* context, void* parent) override {
        currentValue = (int)*field;
        editor = FieldEditorCreator<int>::Create(&currentValue);
        editor->Create(name, context, parent);
    }
    
    void Update(float dt) override {
        if (editor) {
            if (currentValue!=prevValue) {
                prevValue = currentValue;
                *field = (T)currentValue;
            } else {
                currentValue = (int)*field;
                prevValue = currentValue;
            }
            editor->Update(dt);
        }
    }
    
    void Destroy() override {
        if (editor) {
            editor->Destroy();
        }
    }
    
    T* field;
    int currentValue;
    int prevValue;
};



template<typename T>
struct FieldEditorCreator<T, typename std::enable_if< std::is_enum<T>::value >::type> {
    static IFieldEditor* Create(T* ptr) {
        IFieldEditor* editor = new EnumEditor<T>();
        editor->SetField(ptr);
        return editor;
        //std::cout << "Enum editor "<< std::endl;
        //return 0;
    }
};

}
