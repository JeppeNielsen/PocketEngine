//
//  File.cpp
//  Android
//
//  Created by Jeppe Nielsen on 15/07/15.
//
//

#include "File.hpp"
#include "WindowAndroid.hpp"
#include <android/asset_manager.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

using namespace Pocket;

File::File() : size(0), data(0), userData(0) {}
File::File(std::string path) { Load(path); }
File::~File() {
    if (userData) {
        AAsset_close((AAsset*)userData);
    }
 }

bool File::Load(std::string path) {
    
    AAssetManager* mgr = (AAssetManager*)WindowAndroid::assetManager;
    
    if (!mgr) {
        LOGI("AAssetManager not found");
        return false;
    }
    
    const char* filename = path.c_str();
    
    /*
    AAssetDir* dir = AAssetManager_openDir(mgr, "");
    while (true) {
        const char* nextFile = AAssetDir_getNextFileName(dir);
        if (!nextFile) break;
        LOGI("Available asset = %s", nextFile);
    }
    AAssetDir_close(dir);
    */
    
    LOGI("Trying to load file : %s", filename);
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
    if (!asset) {
        LOGI("Asset not found %s", filename);
        return false;
    }
    LOGI("Asset loaded %s", filename);
    userData = asset;
    off_t size = AAsset_getLength(asset);
    data = (unsigned char*)AAsset_getBuffer(asset);
    this->size = (size_t)size;
    return true;
}
