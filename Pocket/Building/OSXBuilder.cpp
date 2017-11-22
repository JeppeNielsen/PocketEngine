//
//  OSXBuilder.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "OSXBuilder.hpp"
#include "ProjectBuilder.hpp"
#include "FileHelper.hpp"
#include <iostream>
#include <fstream>
#include "StringHelper.hpp"
#include "FileReader.hpp"
#include <set>
#include <sstream>
#include "ImageLoader.hpp"
#include "ImageHelper.hpp"

#define FILE_SOURCE(...) #__VA_ARGS__

using namespace Pocket;

void OSXBuilder::Build(const std::string &outputPath, const std::string &pocketEngineLibPath) {

    ProjectBuilder& p = *project;
    std::string gameDir = outputPath+"/"+p.projectName+".app";
    
    if (!FileHelper::CreateFolder(gameDir)) {
        return;
    }
    
    std::string contentDir = gameDir + "/Contents";
    
    if (!FileHelper::CreateFolder(contentDir)) {
        return;
    }
    
    std::string resourcesDir = contentDir + "/Resources";
    
    if (!FileHelper::CreateFolder(resourcesDir)) {
        return;
    }
    
    //resources
    p.CreateResources(resourcesDir+"/resources");
    
    //plist
    CreatePlistInfoFile(contentDir + "/Info.plist");
    
    std::string exeFolder = contentDir + "/MacOS";
    
    if (!FileHelper::CreateFolder(exeFolder)) {
        return;
    }
    
    {
        std::string iconTempFolder = resourcesDir + "/Icon.iconset";
    
        if (!FileHelper::CreateFolder(iconTempFolder)) {
            return;
        }
        
        if (p.iconPath!="" && StringHelper::EndsWith(p.iconPath, ".meta")) {
            std::string path = p.iconPath.substr(0, p.iconPath.size() - 5);
            CreateIcon(path, iconTempFolder);
            FileReader::RunCommmand("rm -r " + iconTempFolder);
        }
    }
    
    std::string outputExe = exeFolder +"/Game";

    p.world->BuildExecutable(pocketEngineLibPath, outputExe, [this] (std::string& code) {
        code += "world.SetLayerScene(0, world.TryFindRoot(\"";
        code += project->startupSceneGUID;
        code += "\"));";
    });
}

void OSXBuilder::CreatePlistInfoFile(const std::string &output) {

std::string plistcontent = FILE_SOURCE(

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleGetInfoString</key>
  <string>__BundleGetInfoString__</string>
  <key>CFBundleExecutable</key>
  <string>__BundleExecutable__</string>
  <key>CFBundleIdentifier</key>
  <string>__BundleIdentifier__</string>
  <key>CFBundleName</key>
  <string>__BundleName__</string>
  <key>CFBundleIconFile</key>
  <string>Icon.icns</string>
  <key>CFBundleShortVersionString</key>
  <string>0.01</string>
  <key>CFBundleInfoDictionaryVersion</key>
  <string>6.0</string>
  <key>CFBundlePackageType</key>
  <string>APPL</string>
  <key>IFMajorVersion</key>
  <integer>0</integer>
  <key>IFMinorVersion</key>
  <integer>1</integer>
</dict>
</plist>
);

    plistcontent = StringHelper::FindAndReplaceAll(plistcontent, "__BundleGetInfoString__", project->projectName);
    plistcontent = StringHelper::FindAndReplaceAll(plistcontent, "__BundleExecutable__", "Game");
    plistcontent = StringHelper::FindAndReplaceAll(plistcontent, "__BundleIdentifier__", project->bundleIdentifier);
    plistcontent = StringHelper::FindAndReplaceAll(plistcontent, "__BundleName__", project->projectName);
    
    std::ofstream file;
    file.open(output);

    file << plistcontent << std::endl;

    file.close();
}

struct ImageSizeFormat {
    std::string name;
    int size;
};

void OSXBuilder::CreateIcon(const std::string &sourceImage, const std::string &destIcon) {

    unsigned char* pixels;
    int width;
    int height;
    
    ImageLoader::TryLoadImage(sourceImage, [&] (unsigned char* p, int w, int h) {
        pixels = p;
        width = w;
        height = h;
    });

    std::vector<ImageSizeFormat> imageFormats = {
        {"icon_16x16.png", 16},
        {"icon_16x16@2x.png", 32},
        {"icon_32x32.png", 32},
        {"icon_32x32@2x.png", 64},
        {"icon_128x128.png", 128},
        {"icon_128x128@2x.png", 256},
        {"icon_256x256.png", 256},
        {"icon_256x256@2x.png", 512},
        {"icon_512x512.png", 512},
        {"icon_512x512@2x.png", 1024},
    };
    
    
    
    for (auto i : imageFormats) {
    unsigned char* scaledPixels = ImageHelper::ScaleImage(pixels, width, height, i.size, i.size);
        ImageHelper::SavePng(destIcon+"/"+i.name, scaledPixels, i.size, i.size);
    
        delete [] scaledPixels;
    }

    FileReader::RunCommmand("iconutil -c icns \""+destIcon+"\"");




}



