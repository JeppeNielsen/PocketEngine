//
//  FacbookClient.hpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/1/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "Event.hpp"
#include "Property.hpp"
#include "FacebookUser.hpp"
#include "HttpRequest.hpp"

using namespace Pocket;

class FacebookClient {
public:
    FacebookClient();
    ~FacebookClient();
    
    enum class Status { NotLoggedIn, NotLoggedInToApp, LoggedIn };
    
    void LogIn(std::string appID);
    
    Property<FacebookClient*, Status> CurrentStatus;
    Event<FacebookClient*> UserUpdated;
    
    void SetAccessToken(std::string token);
    void UpdateCurrentUser();
    
    const FacebookUser& User();
private:
    bool isLoggedIn;
    std::string accessToken;
    FacebookUser user;
    HttpRequest userRequester;
    
    void InvokeUserUpdated(HttpRequest::Request* request);
    
};