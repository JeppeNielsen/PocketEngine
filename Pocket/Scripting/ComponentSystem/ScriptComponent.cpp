//
//  ScriptComponent.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ScriptComponent.hpp"
#include <angelscript.h>
#include "serializer.h"
#include <iostream>
#include "scriptarray.h"
#include <sstream>

using namespace AngelScript;

using namespace Pocket;

ScriptComponent::ScriptComponent() : object(0) {}

void ScriptComponent::Reset() {
    if (object) {
        object->Release();
    }
    object = 0;
}


void ScriptComponent::Store(AngelScript::CSerializer *serializer) {
    serializer->AddExtraObjectToStore(object);
}

void ScriptComponent::Restore(AngelScript::CSerializer *serializer) {
    object = reinterpret_cast<AngelScript::asIScriptObject*>(serializer->GetPointerToRestoredObject(object));
}

void ScriptComponent::Serialize(ISerializedProperty* serializedComponent) {
    SerializeScriptValue(serializedComponent, object, object->GetTypeId(), object->GetObjectType()->GetName(), false);
}

void ScriptComponent::Deserialize(ISerializedProperty* serializedComponent) {
    DeserializeScriptValue(serializedComponent, object, object->GetTypeId(), object->GetObjectType()->GetName(), false);
}

void ScriptComponent::SerializeScriptValue(ISerializedProperty* serializedProperty, void* scriptValue, int typeID, const std::string& name, bool createNode) {

    if (typeID & asTYPEID_OBJHANDLE) {
    
        //std::cout<< name << " = object handle" << std::endl;
       
    } else if (typeID & asTYPEID_SCRIPTOBJECT) {
    
        asIScriptObject* scriptObject = (asIScriptObject *)scriptValue;
        asIObjectType *scriptObjectType = scriptObject->GetObjectType();
        
        ISerializedProperty* serializedObject = createNode ? serializedProperty->Add(name, scriptObject) : serializedProperty;

        for(int i = 0; i < scriptObjectType->GetPropertyCount(); i++ ) {
            int childId;
            const char *childName;
            scriptObjectType->GetProperty(i, &childName, &childId);
            SerializeScriptValue(serializedObject, scriptObject->GetAddressOfProperty(i), childId, childName, true);
        }
   
    } else {
        int primitiveSize = object->GetEngine()->GetSizeOfPrimitiveType(typeID);
        if (primitiveSize>0) {
            if (typeID & asTYPEID_FLOAT){
                SerializePrimitive<float>(serializedProperty, name, scriptValue, primitiveSize);
            } else if (typeID & asTYPEID_INT32){
                SerializePrimitive<int32_t>(serializedProperty, name, scriptValue, primitiveSize);
            }
        } else {
            asIObjectType *type = object->GetEngine()->GetObjectTypeById(typeID);
            int size = type->GetSize();
            if( type ) {
                std::string typeName = type->GetName();
                if (typeName == "string") {
                    SerializePrimitive<std::string>(serializedProperty, name, scriptValue, size);
                } else if (typeName == "array") {
                    CScriptArray* array = static_cast<CScriptArray*>(scriptValue);
                    ISerializedProperty* serializedObject = serializedProperty->Add(name, array);
                    //std::cout<<"size of array : "<< array->GetSize() << std::endl;
                    
                    for (int i=0; i<array->GetSize(); i++) {
                        std::stringstream s;
                        s<<"[" << i << "]";
                        SerializeScriptValue(serializedObject, array->At(i), array->GetElementTypeId(), s.str(), true);
                    }
                    
                }
            }
        }
    }


}


void ScriptComponent::DeserializeScriptValue(ISerializedProperty* serializedProperty, void* scriptValue, int typeID, const std::string& name, bool createNode) {

    if (typeID & asTYPEID_OBJHANDLE) {
    
        //std::cout<< name << " = object handle" << std::endl;
       
    } else if (typeID & asTYPEID_SCRIPTOBJECT) {
    
        asIScriptObject* scriptObject = (asIScriptObject *)scriptValue;
        ISerializedProperty* serializedObject = createNode ? serializedProperty->Get(name) : serializedProperty;
        if (!serializedObject) return;
        
        asIObjectType *scriptObjectType = scriptObject->GetObjectType();

        for(int i = 0; i < scriptObjectType->GetPropertyCount(); i++ ) {
            int childId;
            const char *childName;
            scriptObjectType->GetProperty(i, &childName, &childId);
            
            //ISerializedProperty* serializedChild = serializedObject->Get(childName);
            //if (!serializedChild) continue; // no property with that name
        
            DeserializeScriptValue(serializedObject, scriptObject->GetAddressOfProperty(i), childId, childName, true);
        }
   
    } else {
        int primitiveSize = object->GetEngine()->GetSizeOfPrimitiveType(typeID);
        if (primitiveSize>0) {
            if (typeID & asTYPEID_FLOAT){
                DeserializePrimitive<float>(serializedProperty, name, scriptValue, primitiveSize);
            } else if (typeID & asTYPEID_INT32){
                DeserializePrimitive<int32_t>(serializedProperty, name, scriptValue, primitiveSize);
            }
        } else {
            asIObjectType *type = object->GetEngine()->GetObjectTypeById(typeID);
            int size = type->GetSize();
            if( type ) {
                std::string typeName = type->GetName();
                if (typeName == "string") {
                    DeserializePrimitive<std::string>(serializedProperty, name, scriptValue, size);
                } else if (typeName == "array") {
                    CScriptArray* array = static_cast<CScriptArray*>(scriptValue);
                    ISerializedProperty* serializedObject = serializedProperty->Get(name);
                    if (!serializedObject) return;
                    
                    for (int i=0; i<array->GetSize(); i++) {
                        std::stringstream s;
                        s<<"[" << i << "]";
                        DeserializeScriptValue(serializedObject, array->At(i), array->GetElementTypeId(), s.str(), true);
                    }
                    
                }
            }
        }
    }
}

std::string ScriptComponent::Name() {
    return !object ? "NotDefined" : object->GetObjectType()->GetName();
}