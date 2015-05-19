//
//  FileDownloader.cpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "FileDownloader.hpp"

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <iostream>
#else
#include <stdlib.h>
#include <curl/curl.h>
#endif

using namespace Pocket;

#if !EMSCRIPTEN
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
#endif

FileDownloader::FileDownloader() {
#if !EMSCRIPTEN
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
#endif
}

FileDownloader::~FileDownloader() {
    for (size_t i = 0; i<activeDownloads.size(); i++) {
        delete activeDownloads[i];
    }
    activeDownloads.clear();
#if !EMSCRIPTEN
    curl_easy_cleanup((CURL*)curl);
#endif
}

FileDownloader::Download::Download() {
    url = "";
}

FileDownloader::Download::~Download() {}

#ifdef EMSCRIPTEN

static void onLoad(void* instance, void* data, int size) {
    FileDownloader::Download* download = reinterpret_cast<FileDownloader::Download*>(instance);
    DownloadedFile downloadedFile;
    downloadedFile.url = download->url;
    downloadedFile.data = data;
    downloadedFile.size = size;
    download->Completed(downloadedFile);
    download->container->activeDownloads.erase(std::find(download->container->activeDownloads.begin(), download->container->activeDownloads.end(), download));
    delete download;
}

static void onError(void* instance) {
    FileDownloader::Download* download = reinterpret_cast<FileDownloader::Download*>(instance);
    DownloadedFile downloadedFile;
    downloadedFile.url = download->url;
    downloadedFile.data = 0;
    downloadedFile.size = 0;
    download->Completed(downloadedFile);
    download->container->activeDownloads.erase(std::find(download->container->activeDownloads.begin(), download->container->activeDownloads.end(), download));
    delete download;
}

FileDownloader::Download& FileDownloader::DownloadFile(std::string url) {
    Download* download = new FileDownloader::Download();
    download->container = this;
    download->url = url;
    activeDownloads.push_back(download);
    std::cout<<"Downloading file: " << url << std::endl;
    emscripten_async_wget_data(url.c_str(), download, onLoad, onError);
    return *download;
}


#else

/*
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    
    static int bla = 0;
    bla++;
    
    
    
    FileDownloader::Download* download = reinterpret_cast<FileDownloader::Download*>(userp);
    DownloadedFile downloadedFile;
    downloadedFile.url = download->url;
    downloadedFile.data = contents;
    downloadedFile.size = (int)(size * nmemb);
    download->Completed(downloadedFile);
    //download->container->activeDownloads.erase(std::find(download->container->activeDownloads.begin(), download->container->activeDownloads.end(), download));
    //delete download;
    return size * nmemb;
}
 */

struct MemoryStruct {
    char* memory;
    size_t size;
};


static size_t
WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

FileDownloader::Download& FileDownloader::DownloadFile(std::string url) {
    Download* download = new FileDownloader::Download();
    download->container = this;
    download->url = url;
    activeDownloads.push_back(download);
    
    MemoryStruct mem;
    mem.memory = 0;
    mem.size = 0;
    
    CURL* curl = (CURL*)this->curl;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);
    CURLcode res = curl_easy_perform(curl);
    
    DownloadedFile downloadedFile;
    downloadedFile.url = download->url;
    downloadedFile.data = mem.memory;
    downloadedFile.size = (int)mem.size;
    download->Completed(downloadedFile);
    download->container->activeDownloads.erase(std::find(download->container->activeDownloads.begin(), download->container->activeDownloads.end(), download));
    delete download;
    DownloadComplete(downloadedFile);
    return *download;
}

#endif
