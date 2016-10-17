//
//  Guid.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 15/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Guid.hpp"
#include "StringHelper.hpp"
#ifdef OSX
#include <CoreFoundation/CFUUID.h>
#endif

using namespace Pocket;

Guid::Guid() { }

Guid::Guid(const Guid& other) {
    bytes = other.bytes;
}

Guid::Guid(Guid&& other) {
    bytes = std::forward<decltype(bytes)>(other.bytes);
}

std::string Guid::ToString() const {
    return StringHelper::base64_encode(&bytes[0], 16);
}

void Guid::FromString(const std::string &s) {
    std::string decoded = StringHelper::base64_decode(s);
    for (int i=0; i<16; ++i) {
        bytes[i]=(unsigned char)decoded[i];
    }
}

bool Guid::operator!=(const Pocket::Guid &other) {
    for(int i=0; i<16;++i) {
        if (bytes[i]!=other.bytes[i]) return true;
    }
    return false;
}

bool Guid::operator==(const Pocket::Guid &other) {
    return !(operator!=(other));
}

void Guid::operator=(const Pocket::Guid& other) {
    bytes = other.bytes;
}

Guid Guid::Deserialize(const std::string& data) {
    Guid guid;
    guid.FromString(data);
    return guid;
}

Guid Guid::CreateNew() {
    Guid guid;

#ifdef OSX
    auto newId = CFUUIDCreate(NULL);
    auto bytes = CFUUIDGetUUIDBytes(newId);
    CFRelease(newId);
    
    guid.bytes[0]=bytes.byte0;
    guid.bytes[1]=bytes.byte1;
    guid.bytes[2]=bytes.byte2;
    guid.bytes[3]=bytes.byte3;
    guid.bytes[4]=bytes.byte4;
    guid.bytes[5]=bytes.byte5;
    guid.bytes[6]=bytes.byte6;
    guid.bytes[7]=bytes.byte7;
    guid.bytes[8]=bytes.byte8;
    guid.bytes[9]=bytes.byte9;
    guid.bytes[10]=bytes.byte10;
    guid.bytes[11]=bytes.byte11;
    guid.bytes[12]=bytes.byte12;
    guid.bytes[13]=bytes.byte13;
    guid.bytes[14]=bytes.byte14;
    guid.bytes[15]=bytes.byte15;
#endif
    return guid;
}


