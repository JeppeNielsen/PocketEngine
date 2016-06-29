//
//  ScriptWorld.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "ScriptClass.hpp"
#include "GameWorld.hpp"
#include "TypeInfo.hpp"

namespace Pocket {

class TypeInfo;
class ScriptComponent;
    
class ScriptWorld {
public:

    ScriptWorld();
    
    void SetClangSdkPath(const std::string& clangSdkPath);
    
    void SetFiles(
                  const std::string& dynamicLibPath,
                  const std::string& scriptingIncludeDir,
                  const std::vector<std::string>& sourceFiles,
                  const std::vector<std::string>& headerFiles);
    
    void SetWorldType(GameWorld& world);
    bool Build(bool enableOutput);
    bool LoadLib();
    void UnloadLib();
    
    bool AddGameWorld(GameWorld& world);
    void RemoveGameWorld(GameWorld& world);
    TypeInfo GetTypeInfo(GameObject& object, ComponentID id);
    
    int ComponentCount();
    
    using ScriptComponents = std::map<std::string, int>;
    ScriptComponents Components();
    
    TypeIndexList Types;
    
private:
    void ExtractScriptClasses();
    void WriteMainIncludes(std::ofstream &file);
    void WriteMainCppFile(const std::string& path);
    void WriteMainGameObject(std::ofstream& file);
    void WriteMainSystems(std::ofstream& file);
    void WriteMainComponents(std::ofstream& file);
    void WriteMainSerializedComponents(std::ofstream& file);
    void WriteTypes(std::ofstream& file);
    
    std::string ExtractHeaderPath(const std::string& headerFile);
    std::string ExtractHeaderName(const std::string& headerFile);
    
    bool FindComponentIndex(std::string componentName, bool& staticComponent, int& index);
    
    std::string clangSdkPath;
    std::string dynamicLibPath;
    std::string scriptingIncludeDir;
    std::vector<std::string> sourceFiles;
    std::vector<std::string> headerNames;
    std::vector<std::string> headerPaths;
    
    struct ComponentName {
        std::string name;
        int index;
    };
    
    std::vector<ComponentName> worldComponentNames;
    ScriptClass scriptClasses;
    int componentCount;
    int baseComponentIndex;
    int baseSystemIndex;
    ScriptComponents scriptComponents;
    
    using LibHandle = void*;
    
    LibHandle libHandle;
    
    typedef IGameSystem* (*CreateSystem)(int);
    typedef int (*CountSystems)();
    typedef void (*DeleteSystem)(IGameSystem*);
    
    typedef void* (*CreateComponent)(int);
    typedef int (*CountComponents)();
    typedef void (*DeleteComponent)(int, void*);
    typedef void (*ResetComponent)(int, void*, void*);
    
    typedef TypeInfo* (*GetTypeInfoFunction)(int, void*);
    typedef void (*DeleteTypeInfo)(TypeInfo*);
    
    CreateSystem createSystem;
    CountSystems countSystems;
    DeleteSystem deleteSystem;
    
    CreateComponent createComponent;
    CountComponents countComponents;
    DeleteComponent deleteComponent;
    ResetComponent resetComponent;
    
    GetTypeInfoFunction getTypeInfo;
    DeleteTypeInfo deleteTypeInfo;
    
public:
    friend class ScriptComponent;
};
    
struct ScriptComponent {
    void* data;
    int componentID;
    ScriptWorld* world;
    
    ScriptComponent() : data(0), world(0), componentID(0) { }
    ScriptComponent(ScriptComponent&& other) = delete;
    
    ScriptComponent (const ScriptComponent& other) {
        this->componentID = other.componentID;
        this->world = other.world;
        data = world->createComponent(componentID);
        world->resetComponent(componentID, data, other.data);
    }
    
    ScriptComponent& operator=(const ScriptComponent& other) {
        this->componentID = other.componentID;
        this->world = other.world;
        if (!data) {
            data = world->createComponent(componentID);
        }
        world->resetComponent(componentID, data, other.data);
        return *this;
    }
    
    ~ScriptComponent() {
        if (data) {
            world->deleteComponent(componentID, data);
        }
    }
};
    
}