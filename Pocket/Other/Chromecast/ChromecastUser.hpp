//
//  ChromecastUser.h
//  Chromecast
//
//  Created by Jeppe Nielsen on 12/23/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <string>

namespace Pocket {
  class ChromecastUser {
  public:
    ChromecastUser();
    ~ChromecastUser();
    
    std::string ID;
  };
}