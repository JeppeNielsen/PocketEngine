//
//  atlasCreator.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 26/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AtlasCreator.hpp"
#include <string>
#include "GameWorld.hpp"
#include "FileHelper.hpp"
#include "File.hpp"
#include "ImageLoader.hpp"
#include "RectPacker.hpp"
#include <vector>
#include "Timer.hpp"
#include "Atlas.hpp"
#include <fstream>
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

struct Image {
    int x;
    int y;
    
    unsigned char* pixels;
    int width;
    int height;
    std::string name;
};

bool AtlasCreator::CreateAtlas(const std::string& inputPath, const std::string &texturePath, const std::string &atlasPath, int maxWidth, int maxHeight, const Offsets &offsets) {

    Timer timer;
    
    timer.Begin();

    RectPacker packer;
    
    std::vector<Image> images;
    
    FileHelper::RecurseFolder(inputPath, [&packer, &images, maxWidth, maxHeight] (auto& filename) {
        File imageFile;
        imageFile.Load(filename);
        
        ImageLoader::TryLoadImage(filename, [filename, &packer, &images, maxWidth, maxHeight] (unsigned char* pixels, int width, int height) {
            std::cout << filename << " : "<< width << "x"<<height<< std::endl;
            
            RectPacker::TRect rect;
            rect.w = width + 2;
            rect.h = height + 2;
            packer.AddAtEmptySpotAutoGrow(&rect, maxWidth, maxHeight);
            
            std::string name = FileHelper::GetFileNameFromPath(filename);
            name = name.substr(0, name.length() - 4);
            
            images.push_back({
                rect.x,
                rect.y,
                pixels,
                width,
                height,
                name
            });
        });
    });
    
    unsigned char* finalPixels = new unsigned char[packer.GetW() * packer.GetH() * 4];
    memset(finalPixels, 0, packer.GetW() * packer.GetH() * 4);
    
    for(auto& i : images) {
        for(int x = 0; x<i.width; x++) {
            for(int y = 0; y<i.height; y++) {
                int sourceIndex = x * 4 + y * i.width * 4;
                int destIndex = (i.x + 1 + x) * 4 + (i.y + 1 + y) * packer.GetW() * 4;
                
                finalPixels[destIndex]   = i.pixels[sourceIndex];
                finalPixels[destIndex+1] = i.pixels[sourceIndex+1];
                finalPixels[destIndex+2] = i.pixels[sourceIndex+2];
                finalPixels[destIndex+3] = i.pixels[sourceIndex+3];
            }
        }
        
        //left edge
        {
        int x = 0;
        for(int y = 0; y<i.height; y++) {
            int sourceIndex = x * 4 + y * i.width * 4;
            int destIndex = (i.x + x) * 4 + (i.y + 1 + y) * packer.GetW() * 4;
            
            finalPixels[destIndex]   = i.pixels[sourceIndex];
            finalPixels[destIndex+1] = i.pixels[sourceIndex+1];
            finalPixels[destIndex+2] = i.pixels[sourceIndex+2];
            finalPixels[destIndex+3] = i.pixels[sourceIndex+3];
        }
        }
        
        //right edge
        {
        int x = i.width - 1;
        for(int y = 0; y<i.height; y++) {
            int sourceIndex = x * 4 + y * i.width * 4;
            int destIndex = (i.x + x + 2) * 4 + (i.y + 1 + y) * packer.GetW() * 4;
            
            finalPixels[destIndex]   = i.pixels[sourceIndex];
            finalPixels[destIndex+1] = i.pixels[sourceIndex+1];
            finalPixels[destIndex+2] = i.pixels[sourceIndex+2];
            finalPixels[destIndex+3] = i.pixels[sourceIndex+3];
        }
        }
        
        //top edge
        {
        int y = 0;
        for(int x = 0; x<i.width; x++) {
            int sourceIndex = x * 4 + y * i.width * 4;
            int destIndex = (i.x + 1 + x) * 4 + (i.y + y) * packer.GetW() * 4;
            
            finalPixels[destIndex]   = i.pixels[sourceIndex];
            finalPixels[destIndex+1] = i.pixels[sourceIndex+1];
            finalPixels[destIndex+2] = i.pixels[sourceIndex+2];
            finalPixels[destIndex+3] = i.pixels[sourceIndex+3];
        }
        }
        
        //bottom edge
        {
        int y = i.height - 1;
        for(int x = 0; x<i.width; x++) {
            int sourceIndex = x * 4 + y * i.width * 4;
            int destIndex = (i.x + 1 + x) * 4 + (i.y + y + 2) * packer.GetW() * 4;
            
            finalPixels[destIndex]   = i.pixels[sourceIndex];
            finalPixels[destIndex+1] = i.pixels[sourceIndex+1];
            finalPixels[destIndex+2] = i.pixels[sourceIndex+2];
            finalPixels[destIndex+3] = i.pixels[sourceIndex+3];
        }
        }

        
        
        delete [] i.pixels;
    }
    
    ImageLoader::SaveTga(texturePath, finalPixels, packer.GetW(), packer.GetH());
    
    delete [] finalPixels;
    
    double duration = timer.End();
    
    GameStorage storage;
    storage.AddComponentType<class Atlas>();
    GameWorld world(storage);
    auto root = world.CreateScene();
    auto atlasObject = root->CreateChild();
    auto atlas = atlasObject->AddComponent<class Atlas>();
    
    Vector2 halfPixel = Vector2(0.5f / (float)packer.GetW(), 0.5f / (float)packer.GetH());
    
    for(auto& i : images) {
    
        Box box;
        box.left = (i.x + 1) / (float)packer.GetW() + halfPixel.x;
        box.top = (i.y + 1)  / (float)packer.GetH() + halfPixel.y;
        
        box.right = (i.x + 1 + i.width) / (float)packer.GetW() - halfPixel.x;
        box.bottom = (i.y + 1 + i.height) / (float)packer.GetH() - halfPixel.y;
        
        auto& node = atlas->nodes[i.name];
        node.outer = box;
        
        auto it = offsets.find(i.name);
        
        if (it == offsets.end()) {
            node.inner = box;
        } else {
            Box innerBox;
            innerBox.left = (i.x + 1 + it->second.left) / (float)packer.GetW()+halfPixel.x;
            innerBox.top = (i.y + 1 + it->second.top)  / (float)packer.GetH()+halfPixel.y;
        
            innerBox.right = (i.x + 1 + i.width - it->second.right) / (float)packer.GetW()-halfPixel.x;
            innerBox.bottom = (i.y + 1 + i.height - it->second.bottom) / (float)packer.GetH()-halfPixel.y;
            node.inner = innerBox;
        }
    }
    atlas->textureSize = { (float)packer.GetW(), (float)packer.GetH() };
    
    GameObjectJsonSerializer serializer;
    
    std::ofstream atlasFile;
    atlasFile.open(atlasPath);
    serializer.Serialize(atlasObject, atlasFile);
    atlasFile.close();
    
    std::cout << "Packing took : "<<duration<<" seconds"<<std::endl;
    
    return true;
}
