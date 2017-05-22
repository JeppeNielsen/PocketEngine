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
#include "GameWorld.hpp"
#include "TypeInfo.hpp"
#include "ScriptData.hpp"

namespace Pocket {

class TypeInfo;
class ScriptComponent;
    
class ScriptWorld {
public:

    struct Error {
        int lineNo;
        int lineColumn;
        std::string filename;
        std::string type;
        std::string description;
    };

    ScriptWorld();
    
    void SetClangSdkPath(const std::string& clangSdkPath);
    
    void SetFiles(
                  const std::string& dynamicLibPath,
                  const std::string& scriptingIncludeDir,
                  const std::vector<std::string>& sourceFiles,
                  const std::vector<std::string>& headerFiles);
    
    void SetWorldType(GameWorld& world, const std::function<bool(int)>& componentTypePredicate = 0);
    void Build(bool enableOutput, const std::string &pathToPocketEngineLib, const std::function<void(const Error&)>& onError);
    bool LoadLib();
    void UnloadLib();
    
    bool AddGameWorld(GameWorld& world);
    void AddGameRoot(GameObject* root);
    void AddAllGameRoots(GameWorld& world);
    void RemoveGameRoot(GameObject* root);
    
    void RemoveGameWorld(GameWorld& world);
    TypeInfo GetTypeInfo(const GameObject& object, ComponentId id);
    
    int ComponentCount();
    
    TypeIndexList Types;
    
    bool BuildExecutable(const std::string& pathToPocketEngineLib, const std::string& outputPath, const std::function<void(std::string&)>& customCode, const std::function<void(const std::string&)>& onOutput = nullptr);
    
    void ExtractScriptClasses();
    
    void CheckForErrors(const std::string& file, const std::function<void(const Error& error)>& onError);
    
    int GetBaseComponentIndex();
private:
    void WriteMainIncludes(std::ofstream &file);
    void WriteMainCppFile(const std::string& path);
    void WriteMainGameObject(std::ofstream& file);
    void WriteMainSystems(std::ofstream& file);
    void WriteMainComponents(std::ofstream& file);
    void WriteMainSerializedComponents(std::ofstream& file);
    void WriteSystemIndices(std::ofstream& file);
    void WriteTypes(std::ofstream& file);
    
    void WriteExecutableMain(const std::string& path, const std::function<void(std::string&)>& customCode);
    void WriteExecutableTypeInfos(const std::string &path);
    
    std::string ExtractHeaderPath(const std::string& headerFile);
    std::string ExtractHeaderName(const std::string& headerFile);
    
    bool FindComponentIndex(std::string componentName, bool& staticComponent, int& index);
    
    bool TryParseError(const std::string& codeFile, const std::string& line, Error& error);
    
    using ScriptSystems = std::vector<std::vector<int>>;
    
    ScriptSystems GetScriptSystemsFromPtr(int* ptr);
    
    std::string clangSdkPath;
    std::string dynamicLibPath;
    std::string scriptingIncludeDir;
    std::vector<std::string> sourceFiles;
    std::vector<std::string> headerFiles;
    std::vector<std::string> headerNames;
    std::vector<std::string> headerPaths;
    
    struct ComponentName {
        std::string name;
        int index;
    };
    
    std::vector<ComponentName> worldComponentNames;
    int componentCount;
    int baseComponentIndex;
    int baseSystemIndex;
    ScriptSystems scriptSystems;
    
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
    typedef const char* (*GetComponentName)(int);
    typedef int* (*GetSystems)();
    typedef void (*DeleteGetSystems)(int*);
    
    CreateSystem createSystem;
    CountSystems countSystems;
    DeleteSystem deleteSystem;
    
    CreateComponent createComponent;
    CountComponents countComponents;
    DeleteComponent deleteComponent;
    ResetComponent resetComponent;
    
    GetTypeInfoFunction getTypeInfo;
    DeleteTypeInfo deleteTypeInfo;
    GetComponentName getComponentName;
    
    GetSystems getSystems;
    DeleteGetSystems deleteGetSystems;
    
    ScriptData data;
    
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
