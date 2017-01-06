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

void AssetImporters::OnInitialize() {
    
}

void AssetImporters::OnCreate() {
    AssetImporterSystem* assetImporterSystem = context->ContextRoot().CreateSystem<AssetImporterSystem>();
    
    {
        GameObject* pngImporter = context->ContextRoot().CreateObject();
        pngImporter->AddComponent<AssetImporter>()->extension = "png";
        pngImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<TextureComponent>();
        };
    }
    {
        GameObject* shaderImporter = context->ContextRoot().CreateObject();
        shaderImporter->AddComponent<AssetImporter>()->extension = "shader";
        shaderImporter->GetComponent<AssetImporter>()->OnCreated = [] (GameObject* object){
            object->AddComponent<ShaderComponent>();
        };
    }
    
    context->Project().Opened.Bind([this, assetImporterSystem] () {
       assetImporterSystem->SetPath(context->Project().Path());
    });
}
