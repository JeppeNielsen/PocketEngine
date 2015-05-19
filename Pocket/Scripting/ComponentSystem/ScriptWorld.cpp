//
//  ScriptWorld.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ScriptWorld.hpp"
#include <iostream>
#include <stdio.h>

#include "scriptstdstring.h"
#include "scriptmath.h"
#include <string>
#include <sstream>
#include <assert.h>
#include "GameWorld.hpp"

#include "scriptdictionary.h"
#include "scriptarray.h"
#include "serializer.h"

using namespace AngelScript;
using namespace Pocket;

ScriptWorld::ScriptWorld() {
   
}

ScriptWorld::~ScriptWorld() {
    context->Release();
    engine->Release();
}

void ScriptWorld::Destroy() {
    for (int i=0; i<systems.size(); i++) {
        systems[i]->Destroy();
    }
}

AngelScript::asIScriptEngine* ScriptWorld::Engine() { return engine; }
AngelScript::asIScriptModule* ScriptWorld::Module() { return module; }
AngelScript::asIScriptContext* ScriptWorld::Context() { return context; }

void ScriptWorld::MessageCallback(const AngelScript::asSMessageInfo *msg, void *param) {

    const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

// Function implementation with native calling convention
void ScriptWorld::PrintString(std::string &str)
{
	std::cout << str;
}


GameObject* ScriptWorld::ObjectFactory() {
    return world->CreateObject();
}

void ScriptWorld::RemoveObject(GameObject *object) {
    object->Remove();
}

asIScriptObject* ScriptWorld::AddComponent(int typeID, GameObject *object) {
    ScriptComponentType* componentType = scriptComponentTypes[typeID];
    asIScriptObject* scriptObject = (asIScriptObject*)engine->CreateScriptObject(componentType->scriptType);
    //std::cout << "Added component "<<  componentType->scriptType->GetName() << std::endl;
    ScriptComponent* component = (ScriptComponent*)world->AddComponent(object, componentType->id);
    component->object = scriptObject;
    scriptObject->AddRef();
    return scriptObject;
}

asIScriptObject* ScriptWorld::GetComponent(int typeID, GameObject *object) {
    ScriptComponentType* componentType = scriptComponentTypes[typeID];
    //std::cout << " Get component "<<  componentType->scriptType->GetName() << " typeid: "<< typeID <<std::endl;
    ScriptComponent* component = (ScriptComponent*)object->components[componentType->id];
    if (!component) return 0;
    component->object->AddRef();
    return component->object;
}

int ScriptWorld::ObjectCount() {
    if (!currentSystem) {
        return (int)world->Objects().size();
    } else {
        return (int)currentSystem->Objects().size();
    }
}

GameObject* ScriptWorld::GetObject(int index) {
    if (!currentSystem) {
        return world->Objects()[index];
    } else {
        return currentSystem->Objects()[index];
    }
}

void ScriptWorld::SetActiveSystem(ScriptSystem *system) {
    currentSystem = system;
}

void ScriptWorld::SystemUseComponent(int typeID) {
    if (!currentSystem) return;
    ScriptComponentType* componentType = scriptComponentTypes[typeID];
    world->AddSystem(currentSystem, componentType->id);
}

void TestFunction(asIScriptGeneric *gen) {


}

void ScriptWorld::Initialize(GameWorld* world) {
    this->world = world;
    
    
    
    engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    if( engine == 0 ) {
		std::cout << "Failed to create script engine." << std::endl;
        return;
	}
    engine->SetMessageCallback(asMETHOD(ScriptWorld, MessageCallback), this, asCALL_THISCALL);
    
    RegisterStdString(engine);
    RegisterScriptMath(engine);
    RegisterScriptArray(engine, true);
    RegisterScriptDictionary(engine);
    
    int r = engine->RegisterGlobalFunction("void Print(string &in)", asMETHOD(ScriptWorld, PrintString), asCALL_THISCALL_ASGLOBAL, this); assert( r >= 0 );
    
    
    r = engine->RegisterInterface("GameComponent");
    
    //r = engine->RegisterObjectBehaviour("GameComponent", asBEHAVE_FACTORY, "GameComponent@ f()", asMETHOD(ScriptWorld, ObjectFactory), asCALL_THISCALL_ASGLOBAL, this); assert( r >= 0 );
    
    
    
    
    context = engine->CreateContext();
    
    
    
    r = engine->RegisterObjectType("GameObject", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r>=0);
    
    r = engine->RegisterObjectBehaviour("GameObject", asBEHAVE_FACTORY, "GameObject@ f()", asMETHOD(ScriptWorld, ObjectFactory), asCALL_THISCALL_ASGLOBAL, this); assert( r >= 0 );
    
    r = engine->RegisterObjectMethod("GameObject", "void Remove()", asMETHOD(ScriptWorld, RemoveObject), asCALL_THISCALL_OBJLAST, this); assert( r >= 0 );
    r = engine->RegisterObjectMethod("GameObject", "GameComponent@ AddComponent(int)", asMETHOD(ScriptWorld, AddComponent), asCALL_THISCALL_OBJLAST, this); assert(r>=0);
    r = engine->RegisterObjectMethod("GameObject", "GameComponent@ GetComponent(int)", asMETHOD(ScriptWorld, GetComponent), asCALL_THISCALL_OBJLAST, this); assert(r>=0);
    
    r = engine->RegisterGlobalFunction("int ObjectCount()", asMETHOD(ScriptWorld, ObjectCount), asCALL_THISCALL_ASGLOBAL, this); assert( r >= 0 );
    r = engine->RegisterGlobalFunction("GameObject@ GetObject(int)", asMETHOD(ScriptWorld, GetObject), asCALL_THISCALL_ASGLOBAL, this); assert( r >= 0 );
    
    
    r = engine->RegisterGlobalFunction("void UseComponent(int)", asMETHOD(ScriptWorld, SystemUseComponent), asCALL_THISCALL_ASGLOBAL, this); assert( r >= 0 );
    
    
    r = engine->RegisterInterface("GameSystem"); assert(r>=0);
    r = engine->RegisterInterfaceMethod("GameSystem", "void Initialize()"); assert(r>=0);
    r = engine->RegisterInterfaceMethod("GameSystem", "void ObjectAdded(GameObject@)"); assert(r>=0);
    r = engine->RegisterInterfaceMethod("GameSystem", "void ObjectRemoved(GameObject@)"); assert(r>=0);
    
    r = engine->RegisterGlobalFunction("void TestFunction(?&out)", asFUNCTION(TestFunction), asCALL_GENERIC); assert(r>=0);
    
    
    /*
    r = engine->RegisterEnum("ComponentType"); assert(r>=0);
    
    r = engine->RegisterEnumValue("ComponentType", "Position", 0); assert(r>=0);
    r = engine->RegisterEnumValue("ComponentType", "Velocity", 1); assert(r>=0);
    r = engine->RegisterEnumValue("ComponentType", "Nameable", 2); assert(r>=0);
    */
    
   
    
    mainFunction = 0;
    module = 0;
    
    useSecondModule = false;
}


bool ScriptWorld::Build(std::vector<std::string>& filenames) {

    asIScriptModule* newModule = engine->GetModule(useSecondModule ? "1" : "0", asGM_ALWAYS_CREATE);
    

    scripts.clear();
    for (int i=0; i<filenames.size(); i++) {
        scripts.push_back("");
        if (!AppendScript(filenames[i], scripts.back())) {
            return false;
        }
    }
    
    FindComponentTypeNames();
    
    int r = newModule->AddScriptSection("ComponentType", componentTypesEnum.c_str());
    
    for (int i=0; i<scripts.size(); i++) {
        r = newModule->AddScriptSection(filenames[i].c_str(), &scripts[i][0], scripts[i].length());
        if (r < 0) {
            std::cout << "Failed to add the script: '"<< filenames[i] <<"'." << std::endl;
            return false;
        }
    }
    
    r = newModule->Build();
    if (r < 0) {
        std::cout<<"Compilation failed" << std::endl;
        return false;
    }
    
    useSecondModule = !useSecondModule;
    
    CSerializer serializer;
    
    bool shouldRestore = false;
        
    if (module) {
        
        for (int i=0; i<scriptComponentTypes.size(); i++) {
            auto componentType = scriptComponentTypes[i];
            for (int j=0; j<componentType->components.size(); j++) {
                componentType->components[j]->Store(&serializer);
            }
        }
        
        for (int i=0; i<systems.size(); i++) {
            systems[i]->Store(&serializer);
        }
        serializer.Store(module);
        shouldRestore = true;
    
        for (int i=0; i<scriptComponentTypes.size(); i++) {
            auto componentType = scriptComponentTypes[i];
            for (int j=0; j<componentType->components.size(); j++) {
                componentType->components[j]->object->Release();
            }
        }
        
        for (int i=0; i<systems.size(); i++) {
            systems[i]->scriptObject->Release();
        }
        
        
    }
    
    AngelScript::asIScriptModule* oldModule = module;
    
    module = newModule;
    
    CreateComponentTypes();
    
    if (shouldRestore) {
    
        serializer.Restore(module);
    
        for (int i=0; i<scriptComponentTypes.size(); i++) {
            auto componentType = scriptComponentTypes[i];
            for (int j=0; j<componentType->components.size(); j++) {
                componentType->components[j]->Restore(&serializer);
            }
        }
    }
    
    CreateSystems(&serializer);
    
    if (oldModule) {
        oldModule->Discard();
    }
    
    return true;
}

bool ScriptWorld::CallMain() {
    if (!module) {
        return false;
    }
    
    mainFunction = module->GetFunctionByDecl("void main()");
    if (!mainFunction) {
       std::cout<<"no void main() function found!" << std::endl;
       return false;
    }
    
    context->Prepare(mainFunction);
    return Execute();
}

bool ScriptWorld::AppendScript(std::string filename, std::string &code) {
    
    // We will load the script from a file on the disk.
	FILE *f = fopen(filename.c_str(), "rb");
	if( f == 0 )
	{
		std::cout << "Failed to open the script file '"<< filename<<"'." << std::endl;
		return false;
	}

	// Determine the size of the file	
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);

	// On Win32 it is possible to do the following instead
	// int len = _filelength(_fileno(f));

	// Read the entire file
	std::string script;
	script.resize(len);
	//long c =
    fread(&script[0], len, 1, f);
	fclose(f);
   // if (c == 0) {
     //   std::cout << "Failed to load the script file '"<< filename<<"'." << std::endl;
    //    return false;
    //}
    
    code.append(script);
    return true;
}

bool ScriptWorld::Execute() {
    int r = context->Execute();
	if( r != asEXECUTION_FINISHED )
	{
		// The execution didn't finish as we had planned. Determine why.
		if( r == asEXECUTION_ABORTED ) {
			std::cout << "The script was aborted before it could finish. Probably it timed out." << std::endl;
        }
		else if( r == asEXECUTION_EXCEPTION )
		{
			std::cout << "The script ended with an exception." << std::endl;

			// Write some information about the script exception
			asIScriptFunction *func = context->GetExceptionFunction();
			std::cout << "func: " << func->GetDeclaration() << std::endl;
			std::cout << "modl: " << func->GetModuleName() << std::endl;
			std::cout << "sect: " << func->GetScriptSectionName() << std::endl;
			std::cout << "line: " << context->GetExceptionLineNumber() << std::endl;
			std::cout << "desc: " << context->GetExceptionString() << std::endl;
		}
		else {
			std::cout << "The script ended for some unforeseen reason (" << r << ")." << std::endl;
        }
        return false;
    }
    return true;
}

void ScriptWorld::GetInterfaceTypes(TypeList &list, std::string interfaceName) {
    
    int objectCount = module->GetObjectTypeCount();
    
    for (int i=0; i<objectCount; i++) {
        asIObjectType *type = module->GetObjectTypeByIndex(i);
        for (int j=0; j<type->GetInterfaceCount(); j++) {
            asIObjectType* interface = type->GetInterface(j);
            std::string name = interface->GetName();
            if (name == interfaceName) {
                list.push_back(type);
            }
        }
    }
}

void ScriptWorld::FindComponentTypeNames() {

    std::string componentInterface = "GameComponent";
    
    for (int i=0; i<scripts.size(); i++) {
    
    int counter = 0;
    size_t pos = 0;
    
    std::string& code = scripts[i];
    
    while (true) {
        pos = code.find(componentInterface, pos);
        if (pos == std::string::npos) {
            break;
        }
        //find colon
        size_t colonPos = code.rfind(":", pos);
        
        pos += componentInterface.length(); // move onto next gamecomponent
        
        if (colonPos == std::string::npos) continue; // no colon found, skip to next GameComponent
        
        size_t classPos = code.rfind("class", colonPos);
        if (classPos == std::string::npos) continue; //no class found, skip to next GameComponent
        
        size_t startPos = classPos + 5;
        size_t endPos = colonPos;
        
        std::string componentName = code.substr(startPos, endPos - startPos);
    
        
        std::stringstream trimmer;
        trimmer << componentName;
        componentName.clear();
        trimmer >> componentName;
        
        std::cout<<"Component name found : " << componentName << std::endl;
        
        componentTypeNames[componentName] = counter;
        counter++;
    }
    
    }
    
    std::stringstream s;
    
    for (ComponentTypeNames::iterator it = componentTypeNames.begin(); it!=componentTypeNames.end(); ++it) {
        s<<it->first <<"="<<it->second<<",";
    }
    
    componentTypesEnum = "enum ComponentType { "+s.str()+" }";
    
}

void ScriptWorld::CreateComponentTypes() {
    
    TypeList gameComponentTypes;
    GetInterfaceTypes(gameComponentTypes, "GameComponent");
    
    for (int i=0; i<gameComponentTypes.size(); i++) {
    
        bool alreadyAdded = false;
        for (int j=0; j<scriptComponentTypes.size(); j++) {
            std::string sourceName = scriptComponentTypes[j]->scriptType->GetName();
            std::string sourceNamespace = scriptComponentTypes[j]->scriptType->GetNamespace();
            std::string destName = gameComponentTypes[i]->GetName();
            std::string destNamespace = gameComponentTypes[i]->GetNamespace();
        
            if (sourceName == destName && sourceNamespace == destNamespace) {
                alreadyAdded = true;
                scriptComponentTypes[j]->scriptType = gameComponentTypes[i];
                break;
            }
        }
        if (alreadyAdded) {
            continue;
        }
    
        ScriptComponentType* type = new ScriptComponentType();
        type->id = (int)world->componentTypes.size();
        type->mask = 1<<type->id;
        type->maskInverse = ~type->mask;
        type->index = i;
        
        type->scriptType = gameComponentTypes[i];
        type->name = gameComponentTypes[i]->GetName();
        
        std::cout << "Added script component type "<<  type->scriptType->GetName() <<std::endl;
    
        world->componentTypes.push_back(type);
        scriptComponentTypes.push_back(type);
    }
    
    //int r = engine->RegisterObjectMethod("GameObject", "Velocity@ GetComponent(int)", asMETHOD(ScriptWorld, GetComponent), asCALL_THISCALL_OBJLAST, this); assert(r>=0);
}

void ScriptWorld::CreateSystems(AngelScript::CSerializer* serializer) {
    TypeList scriptSystems;
    GetInterfaceTypes(scriptSystems, "GameSystem");
    
    for (int i=0; i<scriptSystems.size(); i++) {
    
        asIObjectType* systemType = scriptSystems[i];
    
        bool alreadyAdded = false;
        for (int j=0; j<systems.size(); j++) {
            std::string sourceName = scriptSystems[i]->GetName();
            std::string sourceNamespace = scriptSystems[i]->GetNamespace();
            std::string destName = systems[j]->name;
            std::string destNamespace = systems[j]->nameSpace;
        
            if (sourceName == destName && sourceNamespace == destNamespace) {
                alreadyAdded = true;
                
                systems[j]->Restore(serializer);
                
                systems[j]->objectAddedMethod = systemType->GetMethodByDecl("void ObjectAdded(GameObject@)");
                systems[j]->objectRemovedMethod = systemType->GetMethodByDecl("void ObjectRemoved(GameObject@)");
                systems[j]->updateMethod = systemType->GetMethodByDecl("void Update(float)");
                
                
                break;
            }
        }
        if (alreadyAdded) {
            continue;
        }
    
        
        asIScriptFunction *factory =  systemType->GetFactoryByIndex(0); //systemType->GetFactoryByDecl("VelocitySystem @VelocitySystem()");
        
        context->Prepare(factory);
        int r = context->Execute(); assert(r>=0);
        asIScriptObject* scriptObject = reinterpret_cast<asIScriptObject*>(context->GetReturnObject());
        scriptObject->AddRef();
        
        currentSystem = world->CreateSystem<ScriptSystem>();
        currentSystem->scriptWorld = this;
        currentSystem->scriptObject = scriptObject;
        currentSystem->context = context;
        currentSystem->name = scriptObject->GetObjectType()->GetName();
        currentSystem->nameSpace = scriptObject->GetObjectType()->GetNamespace();
        
        systems.push_back(currentSystem);
        
        asIScriptFunction* initializeMethod = systemType->GetMethodByDecl("void Initialize()");
        currentSystem->objectAddedMethod = systemType->GetMethodByDecl("void ObjectAdded(GameObject@)");
        currentSystem->objectRemovedMethod = systemType->GetMethodByDecl("void ObjectRemoved(GameObject@)");
        currentSystem->updateMethod = systemType->GetMethodByDecl("void Update(float)");
        
        
        context->Prepare(initializeMethod);
        context->SetObject(scriptObject);
        r = context->Execute(); assert(r>=0);
        
        currentSystem = 0;
        
        //system->scriptObject = scriptSystems[i];
    }
    
    
}
    
const ScriptWorld::ScriptComponentTypes& ScriptWorld::ComponentTypesList() { return scriptComponentTypes; }

