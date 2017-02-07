//
//  testResources.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "miniz.h"
#include "string.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Timer.hpp"
#include "FileArchive.hpp"
#include "GameWorld.hpp"
#include <fstream>
#include "StringHelper.hpp"
#include "FileHelper.hpp"

using namespace Pocket;

int main_createAssets() {

    std::string zipFile = "/Projects/PocketEngine/Editor/resources.zip";
    
    bool wasCreated = FileArchive::TryCreateArchiveFile("/Projects/PocketEngine/Editor/Pong", zipFile, [] (const std::string& path) -> std::string {
        std::string metaPath = path + ".meta";
        
        if (FileHelper::FileExists(metaPath)) {
            std::ifstream file;
            file.open(metaPath);
            std::string guid = GameWorld::ReadGuidFromJson(file);
            if (guid == "") return "";
            return guid + "-asset";
        } else {
            
            std::ifstream file;
            file.open(path);
            return GameWorld::ReadGuidFromJson(file);
        }
    });
    
    if (!wasCreated) {
        std::cout << "Failed "<<std::endl;
    }
    
    
    FileArchive archive;
    archive.Initialize(zipFile);
    
    for(auto& f : archive.GetFiles()) {
        std::cout << "File : " << f.second.filename << " compressed: " << f.second.compressedSize << " uncompressed: " << f.second.uncompressedSize << " ratio :" << ((float)f.second.compressedSize/f.second.uncompressedSize) << std::endl;
    }
    
    if (!archive.TryLoadData("h93VDOfHRmGf8NnzTYVtyg==", [] (void* data, size_t size) {
        std::stringstream ss;
        ss.write((const char*)data, size);
        
        std::cout << "  "<<  ss.str() << std::endl;
    })) {
        std::cout << "unable to load"<<std::endl;
    }
    /*
    return 0;
    Pocket::Timer timer;
    timer.Begin();
    {
        mz_zip_archive zipArchive;
        memset(&zipArchive, 0, sizeof(zipArchive));
        
        bool succes = mz_zip_writer_init_file(&zipArchive, zipFile.c_str(), 1);
        if (!succes) {
            std::cerr << "mz_zip_writer_init_file failed" << std::endl;
        }
        
        for(int i = 0; i<files.size(); ++i) {
            succes = mz_zip_writer_add_file(&zipArchive, "Button.json", files[i].c_str(), "c", 1, MZ_UBER_COMPRESSION);
            if (!succes) {
                std::cerr << "mz_zip_writer_add_file failed" << std::endl;
            }
        }
        
        succes = mz_zip_writer_finalize_archive(&zipArchive);
        if (!succes) {
            std::cerr << "mz_zip_writer_finalize_archive failed" << std::endl;
        }
        
        succes = mz_zip_writer_end(&zipArchive);
        
        if (!succes) {
            std::cerr << "mz_zip_writer_end failed" << std::endl;
        }
    }
    double time = timer.End();
    std::cout << "Compression took : " <<time << " s" <<std::endl;
    
    {
    
        mz_zip_archive zipArchive;
        memset(&zipArchive, 0, sizeof(zipArchive));
    
        bool succes = mz_zip_reader_init_file(&zipArchive, zipFile.c_str(), 0);
        if (!succes) {
            std::cerr << "mz_zip_reader_init_file failed" << std::endl;
        }
        
        int numberOfFiles = (int)mz_zip_reader_get_num_files(&zipArchive);
        
        for (int i=0; i<1; ++i) {
            mz_zip_archive_file_stat stats;
            memset(&stats, 0, sizeof(stats));
            succes = mz_zip_reader_file_stat(&zipArchive, i, &stats);
            
            if (!succes) {
                std::cerr << "mz_zip_reader_file_stat failed" << std::endl;
            }
            
            size_t bufferSize;
            void* buffer = mz_zip_reader_extract_to_heap(&zipArchive, i, &bufferSize, 0);
            
            std::stringstream ss;
            ss.write((const char*)buffer, bufferSize);
            
            std::cout << "File number : "<< i << "  "<<  ss.str() << std::endl;
            
        }
        
        mz_zip_reader_end(&zipArchive);
    }
    */
    return 0;
}
