//
//  SerializedProperty.hpp
//  Editor
//
//  Created by Jeppe Nielsen on 11/13/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

namespace Pocket {
  
template<class T>
class SerializedProperty;

class ISerializedProperty;

class ISerializedPropertyEditor {
public:
    template<class T>
    T* CreateEditor(ISerializedProperty* property, T* parent) {
        return (T*)CreateEditorInternal(property, parent);
    }
    
    virtual void Started(ISerializedProperty* property, void* editor) = 0;
    virtual void Finished(ISerializedProperty* property, void* editor) = 0;
    virtual void GetValue(ISerializedProperty* property, void* value) = 0;
    virtual void SizeHint(float* width, float* height) {}
    
protected:
    virtual void* CreateEditorInternal(ISerializedProperty* property, void* parent) = 0;
};

template<class Property, class Editor, class Value>
class SerializedPropertyEditor : public ISerializedPropertyEditor {
protected:
    void* CreateEditorInternal(ISerializedProperty* property, void* parent) {
        return CreateEditor(*((Property*)property), static_cast<Editor*>(parent));
    }
    
    void Started(ISerializedProperty* property, void* editor) {
        EditingStarted(*((Property*)property), (Editor*)editor);
    }
    
    void Finished(ISerializedProperty* property, void* editor) {
        EditingFinished(*((Property*)property), (Editor*)editor);
    }
    
    void GetValue(ISerializedProperty* property, void* value) {
        GetValue(*((Property*)property), (Value*)value);
    }

    virtual Editor* CreateEditor(Property& property, Editor* parent) { return 0; }
    virtual void EditingStarted(Property& property, Editor* editor) { }
    virtual void EditingFinished(Property& property, Editor* editor) { }
    virtual void GetValue(Property& property, Value* value) { }
};

template<class T> class SerializedProperty;

class ISerializedProperty {
public:
    ISerializedProperty();
    virtual ~ISerializedProperty();
    
    std::string name;
    
    typedef std::vector<ISerializedProperty*> Children;
    
    template<class T>
    SerializedProperty<T>* Add(std::string name, T value);
    
    ISerializedProperty* Get(std::string name);
    
    ISerializedProperty* GetParent();
    const Children& GetChildren();
    
    virtual ISerializedPropertyEditor* GetEditor();
    
    template<class T>
    T GetValue(std::string name, T defaultValue);
    
private:
    ISerializedProperty* parent;
    Children children;
};

template<class T>
T Pocket::ISerializedProperty::GetValue(std::string name, T defaultValue) {
    Pocket::SerializedProperty<T>* property = dynamic_cast<Pocket::SerializedProperty<T>*>( Get(name) );
    return property ? property->GetValue() : defaultValue;
}

template<class T>
Pocket::SerializedProperty<T>* Pocket::ISerializedProperty::Add(std::string name, T value) {
    SerializedProperty<T>* child = new SerializedProperty<T>();
    child->name = name;
    child->parent = this;
    child->SetValue(value);
    children.push_back(child);
    return child;
}

template<class T>
class SerializedProperty : public ISerializedProperty {
public:
    virtual void SetValue(T value);
    T GetValue();
    
    ISerializedPropertyEditor* GetEditor();
    
    static ISerializedPropertyEditor* Editor;
    
private:
    T value;
};

template<class T>
void SerializedProperty<T>::SetValue(T value) {
    this->value = value;
}

template<class T>
T SerializedProperty<T>::GetValue() { return value; }

template<class T>
ISerializedPropertyEditor* SerializedProperty<T>::Editor = 0;

template<class T>
ISerializedPropertyEditor* SerializedProperty<T>::GetEditor() { return Editor; }

}