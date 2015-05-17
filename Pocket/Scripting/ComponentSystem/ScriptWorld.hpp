//
//  ScriptWorld.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include <angelscript.h>
#include <string>
#include "GameObject.hpp"
#include <map>
#include "ScriptComponentType.hpp"
#include "ScriptSystem.hpp"
#include "serializer.h"

namespace Nano {
  
class GameWorld;

class ScriptWorld {
public:
     ScriptWorld();
     ~ScriptWorld();
    
    void Initialize(GameWorld* world);
    void Destroy();
    bool Build(std::vector<std::string>& filenames);
    
    void Clear();
    
    bool CallMain();
    
    AngelScript::asIScriptEngine* Engine();
    AngelScript::asIScriptModule* Module();
    AngelScript::asIScriptContext* Context();
    
    typedef std::vector<ScriptComponentType*> ScriptComponentTypes;
    
    const ScriptComponentTypes& ComponentTypesList();
    
private:

    bool Execute();

    void FindComponentTypeNames();
    void CreateComponentTypes();
    void CreateSystems(AngelScript::CSerializer* serializer);

    bool AppendScript(std::string filename, std::string& code);
    void MessageCallback(const AngelScript::asSMessageInfo *msg, void *param);
    void PrintString(std::string &str);
    
    GameObject* ObjectFactory();
    void RemoveObject(GameObject* object);
public:
    AngelScript::asIScriptObject* AddComponent(int typeID, GameObject* object);
private:
    AngelScript::asIScriptObject* GetComponent(int typeID, GameObject* object);
    
    int ObjectCount();
    GameObject* GetObject(int index);
    
    void SetActiveSystem(ScriptSystem* system);
    
    void SystemUseComponent(int typeID);
    
    typedef std::vector<AngelScript::asIObjectType*> TypeList;
    void GetInterfaceTypes(TypeList& list, std::string interfaceName);
    
    GameWorld* world;

    std::string componentTypesScriptFilename;
    AngelScript::asIScriptEngine* engine;
    AngelScript::asIScriptModule* module;
    AngelScript::asIScriptContext* context;
    AngelScript::asIScriptFunction* mainFunction;
    
    bool useSecondModule;
    
    typedef std::map<std::string, int> ComponentTypeNames;
    ComponentTypeNames componentTypeNames;
    
    std::string componentTypesEnum;
    
    
    ScriptComponentTypes scriptComponentTypes;
    
    typedef std::vector<ScriptSystem*> Systems;
    Systems systems;
    
    ScriptSystem* currentSystem;
    
    friend class ScriptSystem;
    
    typedef std::vector<std::string> Scripts;
    
    Scripts scripts;
};

}