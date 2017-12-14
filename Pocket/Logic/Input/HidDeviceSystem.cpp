//
//  HidDeviceSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "HidDeviceSystem.hpp"
#include "hidapi.h"

using namespace Pocket;

void HidDeviceSystem::Initialize() {
    hid_init();
}

void HidDeviceSystem::Destroy() {
    hid_exit();
}

void HidDeviceSystem::ObjectAdded(Pocket::GameObject *object) {
    HidDevice* usbDevice = object->GetComponent<HidDevice>();
    hid_device* device = hid_open(usbDevice->VendorId, usbDevice->ProductId, NULL);
    devices[usbDevice] = device;
}

void HidDeviceSystem::ObjectRemoved(Pocket::GameObject *object) {
    HidDevice* usbDevice = object->GetComponent<HidDevice>();
    auto it = devices.find(usbDevice);
    hid_device* device = (hid_device*)it->second;
    hid_close(device);
    devices.erase(it);
}

void HidDeviceSystem::Update(float dt) {
    for(auto o : Objects()) {
        UpdateDevice(o->GetComponent<HidDevice>());
    }
}

void HidDeviceSystem::UpdateDevice(Pocket::HidDevice *hidDevice) {
    if (hidDevice->data.empty()) {
        return;
    }
    hid_device* device = (hid_device*)devices[hidDevice];
    if (!device) {
        return;
    }
    
    hidDevice->prevData = hidDevice->data;
    hid_read_timeout(device, &hidDevice->data[0], hidDevice->data.size(), 0);
    
    for (int i=0; i<hidDevice->data.size(); ++i) {
        std::cout << "["<< i <<"]"<<" = "<<(int)hidDevice->data[i]<<std::endl;
    }
}

