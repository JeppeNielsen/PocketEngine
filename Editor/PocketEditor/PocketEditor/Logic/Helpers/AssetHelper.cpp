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
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

bool AssetHelper::CreateFolder(const std::string& path) {
    return mkdir(path.c_str(), 0777) == 0;
}

bool AssetHelper::CreateScene(const std::string &path) {

    GameStorage storage;
    storage.AddComponentType<Transform>();
    storage.AddComponentType<Mesh>();
    storage.AddComponentType<Renderable>();
    
    GameWorld world(storage);
    
    GameObject* root = world.CreateScene();
    
    GameObject* cube = root->CreateObject();
    cube->AddComponent<Transform>();
    cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    cube->AddComponent<Renderable>();
    
    GameObjectJsonSerializer serializer;
    
    std::ofstream file;
    file.open(path);
    serializer.Serialize(root, file);
    file.close();

    return true;
}

bool AssetHelper::CreateEmptyPrefab(const std::string& path) {
    GameStorage storage;
    GameWorld world(storage);
    
    GameObject* prefab = world.CreateScene();
    
    GameObjectJsonSerializer serializer;
    
    std::ofstream file;
    file.open(path);
    serializer.Serialize(prefab, file);
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

bool AssetHelper::CreateShader(const std::string &path) {
    std::string content = "";
    content += "{\n";
    content += "    attribute vec4 Position;\n";
    content += "    attribute vec4 Color;\n";
    content += "    attribute vec3 Normal;\n";
    content += "    attribute vec2 TexCoords;\n";
    content += "    uniform mat4 ViewProjection;\n";
    content += "    varying vec4 vColor;\n";
    content += "    varying vec3 vNormal;\n";
    content += "    varying vec2 vTexCoords;\n";
    content += "    void main() {\n";
    content += "        vColor = Color;\n";
    content += "        vNormal = Normal;\n";
    content += "        vTexCoords = TexCoords;\n";
    content += "        gl_Position = Position * ViewProjection;\n";
    content += "    }\n";
    content += "}\n";
    content += "{\n";
    content += "    varying vec4 vColor;\n";
    content += "    varying vec3 vNormal;\n";
    content += "    varying vec2 vTexCoords;\n";
    content += "    uniform sampler2D Texture;\n";
    content += "    void main() {\n";
    content += "        vec4 textureColor = texture2D(Texture, vTexCoords);\n";
    content += "        float spec = 0.2 + clamp(dot(vNormal, vec3(0.0,1.0,0.0)),0.0,1.0) * 0.8;\n";
    content += "        gl_FragColor = vec4(textureColor.r * spec, textureColor.g * spec, textureColor.b * spec,textureColor.a);\n";
    content += "    }\n";
    content += "}\n";
    
    return CreateTextFile(path, content);
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
