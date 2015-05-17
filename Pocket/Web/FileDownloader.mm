//
//  FileDownloader.cpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "FileDownloader.hpp"

#include <Foundation/Foundation.h>

using namespace Nano;

FileDownloader::FileDownloader() {

}

FileDownloader::~FileDownloader() {
    for (size_t i = 0; i<activeDownloads.size(); i++) {
        delete activeDownloads[i];
    }
    activeDownloads.clear();
}

FileDownloader::Download::Download() {
    url = "";
}

FileDownloader::Download::~Download() {}




FileDownloader::Download& FileDownloader::DownloadFile(std::string url) {
    Download* download = new FileDownloader::Download();
    download->container = this;
    download->url = url;
    activeDownloads.push_back(download);
    
    NSString* str = [[NSString alloc]initWithUTF8String:url.c_str()];
    /*
    NSURLSession *session = [NSURLSession sharedSession];
    [[session dataTaskWithURL:[NSURL URLWithString:str]
            completionHandler:^(NSData *data,
                                NSURLResponse *response,
                                NSError *error) {
                DownloadedFile downloadedFile;
                downloadedFile.url = download->url;
                downloadedFile.data = (void*)data.bytes;
                downloadedFile.size = (int)data.length;
                download->Completed(downloadedFile);
                download->container->activeDownloads.erase(std::find(download->container->activeDownloads.begin(), download->container->activeDownloads.end(), download));
                delete download;
                DownloadComplete(downloadedFile);
             
    }] resume];
     */
    
    NSURLRequest *request = [NSURLRequest requestWithURL:
                             [NSURL URLWithString:str]];
    
    [NSURLConnection sendAsynchronousRequest:request
                                       queue:[NSOperationQueue mainQueue]
                           completionHandler:^(NSURLResponse *response,
                                               NSData *data,
                                               NSError *connectionError) {
                               // handle response
                               
                               DownloadedFile downloadedFile;
                               downloadedFile.url = download->url;
                               downloadedFile.data = (void*)data.bytes;
                               downloadedFile.size = (int)data.length;
                               download->Completed(downloadedFile);
                               download->container->activeDownloads.erase(std::find(download->container->activeDownloads.begin(), download->container->activeDownloads.end(), download));
                               delete download;
                               DownloadComplete(downloadedFile);
                               
                           }];
    
    
    
    return *download;
}

