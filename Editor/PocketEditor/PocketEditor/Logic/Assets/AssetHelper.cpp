//
//  AssetHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AssetHelper.hpp"
#include <sys/stat.h>
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Renderable.hpp"
#include "GameWorld.hpp"
#include <fstream>
#include "FileReader.hpp"
#include "StringHelper.hpp"
#include "FileHelper.hpp"

using namespace Pocket;

bool AssetHelper::CreateFolder(const std::string& path) {
    return mkdir(path.c_str(), 0777) == 0;
}

bool AssetHelper::CreateScene(const std::string &path) {
    GameWorld world;
    
    GameObject* root = world.CreateRoot();
    
    GameObject* cube = root->CreateObject();
    cube->AddComponent<Transform>();
    cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    cube->AddComponent<Renderable>();
    
    std::ofstream file;
    file.open(path);
    root->ToJson(file);
    file.close();

    return true;
}

bool AssetHelper::CreateEmptyPrefab(const std::string& path) {
    GameWorld world;
    GameObject* root = world.CreateRoot();
    
    std::ofstream file;
    file.open(path);
    root->ToJson(file);
    file.close();
    return true;
}

bool AssetHelper::CreateHeader(const std::string& path) {
    
    
    
    return true;
}

bool AssetHelper::CreateSource(const std::string& path) {

    return true;
}

#define FILE_SOURCE(...) #__VA_ARGS__

bool AssetHelper::CreateComponent(const std::string& path) {

    std::string filename = FileHelper::GetFileNameFromPath(path);
    
    std::string headerFile = path + ".hpp";
    std::string sourceFile = path + ".cpp";
    
    std::string systemName = filename + "System";
    
    std::string headerContent = "#pragma once\n";
    headerContent += "#include \"GameSystem.hpp\"\n";
    headerContent += "\n";
    headerContent += "using namespace Pocket;\n";
    headerContent += "\n";
    headerContent += "struct " + filename + " { \n";
    headerContent += "   float variable;";
    headerContent += "\n";
    headerContent += "};\n";
    headerContent += "\n";
    headerContent += "struct " + systemName + " : GameSystem<"+filename+"> {\n";
    headerContent += "   void ObjectAdded(GameObject* object) override;\n";
    headerContent += "   void ObjectRemoved(GameObject* object) override;\n";
    headerContent += "   void Update(float dt) override;\n";
    headerContent += "};\n";
    headerContent += "\n";
    
    std::string sourceContent = "#include \"" + filename + ".hpp\"\n";
    sourceContent += "\n";
    sourceContent += "void "+systemName+"::ObjectAdded(GameObject* object) {\n";
    sourceContent += "\n";
    sourceContent += "}\n";

    sourceContent += "\n";
    sourceContent += "void "+systemName+"::ObjectRemoved(GameObject* object) {\n";
    sourceContent += "\n";
    sourceContent += "}\n";
    
    sourceContent += "\n";
    sourceContent += "void "+systemName+"::Update(float dt) {\n";
    sourceContent += "\n";
    sourceContent += "}\n";
    
    if (!CreateTextFile(headerFile,headerContent)) return false;
    if (!CreateTextFile(sourceFile, sourceContent)) return false;
    return true;
}

bool AssetHelper::TryCreateAsset(const std::string &path, const std::string &message, const std::string &defaultName, const std::string& extension, std::function<void (const std::string &)> onCreate) {
    std::string name = FileReader::ShowMessageTextBox(message, defaultName);
    if (name!="") {
        if (extension!="" && !StringHelper::EndsWith(name, extension)) {
            name += extension;
        }
        onCreate((StringHelper::EndsWith(path, "/") ? path : path + "/" ) + name);
        return true;
    } else {
        return false;
    }
}

bool AssetHelper::CreateTextFile(const std::string &path, const std::string &content) {
    std::ofstream file;
    file.open(path);
    file << content;
    
    return true;
}
