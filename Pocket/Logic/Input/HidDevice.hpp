//
//  UsbDevice.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <vector>

namespace Pocket {
    struct HidDevice {
        unsigned short VendorId;
        unsigned short ProductId;
        
        using Data = std::vector<unsigned char>;
        Data data;
        Data prevData;
    };
}
