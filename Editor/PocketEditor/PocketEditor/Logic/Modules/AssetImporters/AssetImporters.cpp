//
//  AssetImporters.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "AssetImporters.hpp"
#include "AssetImporterSystem.hpp"
#include "ShaderComponent.hpp"
#include "AssetLoader.hpp"

void AssetImporters::OnInitialize() {
    
}

void AssetImporters::OnCreate() {
    AssetImporterSystem* assetImporterSystem = context->GuiRoot().CreateSystem<AssetImporterSystem>();
    assetImporterSystem->SetFileWatcher(context->Project().FileSystemWatcher());
    {
        GameObject* pngImporter = context->GuiRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "png";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<TextureComponent>();
        };
    }
    {
        GameObject* pngImporter = context->GuiRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "psd";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<TextureComponent>();
        };
    }
    assetImporterSystem->SetFileWatcher(context->Project().FileSystemWatcher());
    {
        GameObject* pngImporter = context->GuiRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "jpg";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<TextureComponent>();
        };
    }
    {
        GameObject* shaderImporter = context->GuiRoot().CreateObject();
        shaderImporter->AddComponent<AssetImporter>()->extension = "shader";
        shaderImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<ShaderComponent>();
        };
    }
    {
        GameObject* trueTypeImporter = context->GuiRoot().CreateObject();
        trueTypeImporter->AddComponent<AssetImporter>()->extension = "ttf";
        trueTypeImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<Font>();
            object->AddComponent<TextureComponent>();
        };
    }
    {
        GameObject* trueTypeImporter = context->GuiRoot().CreateObject();
        trueTypeImporter->AddComponent<AssetImporter>()->extension = "otf";
        trueTypeImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<Font>();
            object->AddComponent<TextureComponent>();
        };
    }
}
