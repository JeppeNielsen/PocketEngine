//
//  ChromecastReceiver.h
//  Chromecast
//
//  Created by Jeppe Nielsen on 12/23/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Event.hpp"
#include <string>
#include <vector>
#include "ChromecastMessage.hpp"
#include "ChromecastUser.hpp"

namespace Pocket {
  class ChromecastReceiver {
  public:
    ChromecastReceiver();
    ~ChromecastReceiver();
    
    void Initialize();
    
    Event<ChromecastMessage> MessageReceived;
    Event<ChromecastUser*> UserConnected;
    Event<ChromecastUser*> UserDisconnected;
    
private:
    std::string channelID;
    
    typedef std::vector<ChromecastUser> Users;
    Users users;
public:
    void ConnectUser();
    void DisconnectUser();
    void ReceiveMessage(std::string data);
    
  };
}