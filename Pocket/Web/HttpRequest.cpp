//
//  HttpRequest.cpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/1/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#if EMSCRIPTEN
#include <emscripten/emscripten.h>
#else
#include <curl/curl.h>
#endif

#include "HttpRequest.hpp"
#include <iostream>
#include <sstream>

using namespace Nano;

#if !EMSCRIPTEN
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
#endif

HttpRequest::HttpRequest() : Progress(this), Status(this) {
    Progress = 0.0f;
    Status = RequestStatus::Pending;
    id = idCounter++;
#if !EMSCRIPTEN
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
#endif
}

HttpRequest::~HttpRequest() {
#if !EMSCRIPTEN
    curl_easy_cleanup((CURL*)curl);
#endif
}

int HttpRequest::idCounter = 0;

void HttpRequest::AddPostParameter(std::string key, std::string value) {
    if (postParameters.size() > 0) {
        postParameters += "&";
        postParameters += key;
        postParameters += "=";
        postParameters += value;
    } else {
        postParameters += key;
        postParameters += "=";
        postParameters += value;
    }
}

void HttpRequest::ClearPostParameters() {
    postParameters.clear();
}

void HttpRequest::OnProgress(int progress) {
    Progress = ((float)progress / 100.0f);
    //std::cout<<"DoRequest progress changed " << Progress() << std::endl;
}

void HttpRequest::OnError(int error) {
    Request request;
    request.httpRequest = this;
    request.error = error;
    request.url = url;
    request.data = "";
    Status = RequestStatus::Failed;
    Progress = 1.0f;
    std::cout<<"DoRequest failed  error:" << error << std::endl;
    Failed(&request);
}

#if EMSCRIPTEN

void HttpRequest::onLoaded(void* parent, const char * file) {
	HttpRequest* req = reinterpret_cast<HttpRequest*>(parent);
	req->OnLoaded(file);
}

void HttpRequest::onError(void* parent, int statuserror) {
	HttpRequest* req = reinterpret_cast<HttpRequest*>(parent);
	req->OnError(statuserror);
}

void HttpRequest::onProgress(void* parent, int progress) {
	HttpRequest* req = reinterpret_cast<HttpRequest*>(parent);
	req->OnProgress(progress);
}

void HttpRequest::DoRequest(std::string url, RequestType requestType, std::string targetFilename) {
    this->url = url;
    std::cout<<"DoRequest " << url << std::endl;
    
    if (targetFilename.size() == 0 ) {
        std::stringstream s;
        s<<"prepare"<<id;
        targetFilename = s.str();
    }
    
    file = targetFilename;
    
    std::cout<<"DoRequest file: " << targetFilename << std::endl;
    
    emscripten_async_wget2(
                           url.c_str(),
                           targetFilename.c_str(),
                           (requestType==RequestType::Get) ? "GET" : "POST",
                           postParameters.c_str(),
                           this,
                           HttpRequest::onLoaded, HttpRequest::onError, HttpRequest::onProgress
                           );
}



void HttpRequest::OnLoaded(const char *f) {
    Request request;
    request.httpRequest = this;
    request.error = 0;
    request.url = url;
    request.data = "";
    const char* file = this->file.c_str();
    
    if (strstr(file,"prepare")) {
		FILE* f = fopen(file,"rb");
		if (f) {
			fseek (f, 0, SEEK_END);
    		int size=ftell (f);
    		fseek (f, 0, SEEK_SET);
            char* data = new char[size];
			fread(data,size,1,f);
			request.data = data;
			delete data;
			fclose(f);
		} else {
			request.data = file;
		}
	} else {
		request.data = file;
	}
    std::cout<<"DoRequest success " << request.data << std::endl;
    
	Status = RequestStatus::OK;
    Progress = 1.0f;
    Success(&request);
}

#else

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    HttpRequest* instance = reinterpret_cast<HttpRequest*>(userp);
    instance->OnLoaded(contents, size * nmemb);
   
    
    return size * nmemb;
}

void HttpRequest::OnLoaded(void* contents, size_t size) {
    Request request;
    request.url = this->url;
    request.error = 0;
    request.data = "";
    request.data.append((char*)contents, size);
    Progress = 1;
    Success(&request);
}

void HttpRequest::DoRequest(std::string url, RequestType requestType, std::string targetFilename) {
    this->url = url;
    std::cout<<"DoRequest " << url << std::endl;
    
    Progress = 0;
    
    CURL* curl = (CURL*)this->curl;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    CURLcode res = curl_easy_perform(curl);
}

#endif

