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
    AssetImporterSystem* assetImporterSystem = context->ContextRoot().CreateSystem<AssetImporterSystem>();
    assetImporterSystem->SetFileWatcher(context->Project().FileSystemWatcher());
    {
        GameObject* pngImporter = context->ContextRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "png";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<TextureComponent>();
        };
    }
    {
        GameObject* pngImporter = context->ContextRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "psd";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<TextureComponent>();
        };
    }
    assetImporterSystem->SetFileWatcher(context->Project().FileSystemWatcher());
    {
        GameObject* pngImporter = context->ContextRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "jpg";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<TextureComponent>();
        };
    }
    {
        GameObject* shaderImporter = context->ContextRoot().CreateObject();
        shaderImporter->AddComponent<AssetImporter>()->extension = "shader";
        shaderImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<ShaderComponent>();
        };
    }
    {
        GameObject* trueTypeImporter = context->ContextRoot().CreateObject();
        trueTypeImporter->AddComponent<AssetImporter>()->extension = "ttf";
        trueTypeImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<AssetLoader>();
            object->AddComponent<Font>();
            object->AddComponent<TextureComponent>();
        };
    }
}
