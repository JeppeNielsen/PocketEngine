//
//  FileDownloader.hpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "Event.hpp"
#include <vector>

namespace Nano {

    struct DownloadedFile {
        std::string url;
        void* data;
        int size;
    };
    
    class FileDownloader {
    public:
        
        FileDownloader();
        ~FileDownloader();
   
       
        class Download {
        public:
            Download();
            ~Download();
            
            FileDownloader* container;
            
            std::string url;
            Event<DownloadedFile> Completed;
        };
        
        Download& DownloadFile(std::string url);
        
        typedef std::vector<struct Download*> DownloadList;
        DownloadList activeDownloads;
        
        Event<DownloadedFile> DownloadComplete;
        
#if !EMSCRIPTEN
        void* curl;
#endif
    };
}