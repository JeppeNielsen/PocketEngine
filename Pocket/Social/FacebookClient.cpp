//
//  FacbookClient.cpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/1/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "FacebookClient.hpp"

#if EMSCRIPTEN
#include <emscripten/emscripten.h>
#else

#endif

#include <iostream>

static FacebookClient* instance = 0;
static int FacebookStatus = 0;

#if EMSCRIPTEN

extern "C" {
    
    int facebookChangedStatus(int status, const char* accessToken) {
        std::cout<< "facebookChangedStatus status: "<<status << std::endl;
        FacebookStatus = status;
        if (!instance) return 0;

        
        instance->SetAccessToken(accessToken);
        instance->CurrentStatus = (FacebookClient::Status)status;
        
        if (instance->CurrentStatus == FacebookClient::Status::LoggedIn) {
            instance->UpdateCurrentUser();
        }
        
        
        //std::string url = "/me?access_token=" + std::string(accessToken);
        //http* http = new class http("https://graph.facebook.com",http::REQUEST_GET);
        //http->runRequest(url.c_str(),http::ASSYNC_THREAD);
        
         
        return 1;
    }
}

#endif

FacebookClient::FacebookClient() : CurrentStatus(this) { instance = this; userRequester.Success += event_handler(this, &FacebookClient::InvokeUserUpdated); }
FacebookClient::~FacebookClient() {}

//appid : 262227457314085

void FacebookClient::LogIn(std::string appID) {
#if EMSCRIPTEN
    EM_ASM(
        LogInToFacebook('262227457314085');
    );
#endif
    CurrentStatus = (FacebookClient::Status)FacebookStatus;
}

void FacebookClient::SetAccessToken(std::string token) {
    accessToken = token;
}

void FacebookClient::UpdateCurrentUser() {
    std::cout<<"Trying to update user access_token="<< accessToken << std::endl;
    userRequester.DoRequest("https://graph.facebook.com/me?access_token=" + accessToken);
}

void FacebookClient::InvokeUserUpdated(HttpRequest::Request *request) {
    user.UpdateFromJson(request->data);
    UserUpdated(this);
}

const FacebookUser& FacebookClient::User() { return user; }

