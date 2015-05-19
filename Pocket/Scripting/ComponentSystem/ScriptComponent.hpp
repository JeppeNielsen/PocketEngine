//
//  ScriptComponent.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "SerializedProperty.hpp"
#include <angelscript.h>

namespace AngelScript {
    class asIScriptObject;
    class CSerializer;
}

namespace Pocket {
  
class ScriptComponent : public GameComponent<ScriptComponent> {
public:
    ScriptComponent();
    void Reset();
    AngelScript::asIScriptObject* object;
    
    void Store(AngelScript::CSerializer* serializer);
    void Restore(AngelScript::CSerializer* serializer);
    
    void Serialize(ISerializedProperty* serializedComponent);
    void Deserialize(ISerializedProperty* serializedComponent);

    virtual std::string Name();
    
private:
    void SerializeScriptValue(ISerializedProperty* serializedProperty, void* scriptValue, int typeID, const std::string& name, bool createNode);
    void DeserializeScriptValue(ISerializedProperty* serializedProperty, void* scriptValue, int typeID, const std::string& name, bool createNode);
    
    
    template<class T>
    void SerializePrimitive(ISerializedProperty* serializedComponent, const std::string& name, void* adressOfProperty, int primitiveSize) {
        T value;
        memcpy(&value, adressOfProperty, primitiveSize);
        serializedComponent->Add(name, value);
    }
    
    template<class T>
    void DeserializePrimitive(ISerializedProperty* serializedComponent, const std::string& name, void* adressOfProperty, int primitiveSize) {
        ISerializedProperty* serializedProperty = serializedComponent->Get(name);
        if (!serializedProperty) return;
        SerializedProperty<T>* realProperty = dynamic_cast<SerializedProperty<T>*>(serializedProperty);
        if (!realProperty) return; // property type does not match, might happen
        T value = realProperty->GetValue();
        memcpy(adressOfProperty, &value, primitiveSize);
    }
    
};

}