//
//  HttpRequest.hpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/1/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "Event.hpp"
#include "Property.hpp"

namespace Pocket {
enum class RequestType { Get, Post };
enum class RequestStatus { Pending, OK, Failed };
    
class HttpRequest {
public:
    HttpRequest();
    ~HttpRequest();
    
    void DoRequest(std::string url, RequestType requestType = RequestType::Get, std::string targetFileName = "");
  
    void AddPostParameter(std::string key, std::string value);
    void ClearPostParameters();
    
    static void onLoaded(void* parent, const char * file);
    static void onError(void* parent, int statuserror);
    static void onProgress(void* parent, int progress);
      
    struct Request {
        HttpRequest* httpRequest;
        std::string url;
        std::string data;
        int error;
    };
    
    void OnProgress(int progress);
    void OnError(int error);
    
    Event<Request*> Success;
    Event<Request*> Failed;
    
    Property<HttpRequest*, float> Progress;
    Property<HttpRequest*, RequestStatus> Status;
    
private:
    std::string postParameters;
    std::string url;
    std::string file;
    int id;
    static int idCounter;
    
#if EMSCRIPTEN
public:
    void OnLoaded(const char* file);
#else
public:
    void OnLoaded(void* contents, size_t size);
private:
    void* curl;
#endif
    
};
}