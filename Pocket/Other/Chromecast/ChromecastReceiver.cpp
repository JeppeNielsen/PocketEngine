//
//  ChromecastReceiver.cpp
//  Chromecast
//
//  Created by Jeppe Nielsen on 12/23/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ChromecastReceiver.hpp"
#include <iostream>
#if EMSCRIPTEN
#include <emscripten/emscripten.h>
#else

#endif

using namespace Pocket;

static ChromecastReceiver* instance = 0;

ChromecastReceiver::ChromecastReceiver() { instance = this; }
ChromecastReceiver::~ChromecastReceiver() { instance = 0; }


void ChromecastReceiver::Initialize() {
#if EMSCRIPTEN
    EM_ASM(
        CreateReceiver();
    );
#endif
}


#if EMSCRIPTEN

extern "C" {
    
    int chromecastMessageReceived(const char* data) {
        std::cout<< "chromecastMessageReceived message: "<< data << std::endl;
        if (!instance) return 0;
        instance->ReceiveMessage(data);
        return 1;
    }
}

#endif

void ChromecastReceiver::ReceiveMessage(std::string data) {

    ChromecastMessage message;
    message.data = data;

    MessageReceived(message);
}


void ChromecastReceiver::ConnectUser() {

}

void ChromecastReceiver::DisconnectUser() {

}