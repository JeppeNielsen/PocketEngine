//
//  BigButtonManager.cpp
//  QuizMaster
//
//  Created by Jeppe Nielsen on 3/31/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#include "BigButtonManager.hpp"

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>
#include <iostream>

using namespace Pocket;

BigButtonManager::BigButtonManager() : device(0), interface(0) {
    thread.Code += event_handler(this, &BigButtonManager::PollLoop);
    
    for (int i=0; i<4; i++) {
        controller[i].timeSinceButtonClick = 0;
        controller[i].buttonsChanged = false;
    }
}

BigButtonManager::~BigButtonManager() { }

bool BigButtonManager::Initialize() {
    
CFMutableDictionaryRef matchingDictionary = NULL;
SInt32 idVendor = 0x045e; // set vendor id
SInt32 idProduct = 0x02a0; // set product id

io_iterator_t iterator = 0;
io_service_t usbRef;
SInt32 score;
IOCFPlugInInterface** plugin;
IOUSBDeviceInterface300** usbDevice = NULL;
IOReturn ret;
IOUSBConfigurationDescriptorPtr config;
IOUSBFindInterfaceRequest interfaceRequest;
IOUSBInterfaceInterface300** usbInterface;


matchingDictionary = IOServiceMatching(kIOUSBDeviceClassName);
CFDictionaryAddValue(matchingDictionary,
                     CFSTR(kUSBVendorID),
                     CFNumberCreate(kCFAllocatorDefault,
                                    kCFNumberSInt32Type, &idVendor));
CFDictionaryAddValue(matchingDictionary,
                     CFSTR(kUSBProductID),
                     CFNumberCreate(kCFAllocatorDefault,
                                    kCFNumberSInt32Type, &idProduct));
IOServiceGetMatchingServices(kIOMasterPortDefault,
                             matchingDictionary, &iterator);
usbRef = IOIteratorNext(iterator);
if (usbRef == 0)
{
    printf("Device not found\n");
    return false;
}
IOObjectRelease(iterator);
IOCreatePlugInInterfaceForService(usbRef, kIOUSBDeviceUserClientTypeID,
                                  kIOCFPlugInInterfaceID, &plugin, &score);
IOObjectRelease(usbRef);
(*plugin)->QueryInterface(plugin,
                          CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID300),
                          (LPVOID*)&usbDevice);
(*plugin)->Release(plugin);


ret = (*usbDevice)->USBDeviceOpen(usbDevice);
if (ret == kIOReturnSuccess)
{
    // set first configuration as active
    ret = (*usbDevice)->GetConfigurationDescriptorPtr(usbDevice, 0, &config);
    if (ret != kIOReturnSuccess)
    {
        printf("Could not set active configuration (error: %x)\n", ret);
        return false;
    }
    (*usbDevice)->SetConfiguration(usbDevice, config->bConfigurationValue);
    
    printf("config numberInterfaces= %x \n", config->bNumInterfaces);
}
else if (ret == kIOReturnExclusiveAccess)
{
    // this is not a problem as we can still do some things
}
else
{
    printf("Could not open device (error: %x)\n", ret);
    return false;
}

interfaceRequest.bInterfaceClass = kIOUSBFindInterfaceDontCare;
interfaceRequest.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
interfaceRequest.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
interfaceRequest.bAlternateSetting = kIOUSBFindInterfaceDontCare;
(*usbDevice)->CreateInterfaceIterator(usbDevice,
                                      &interfaceRequest, &iterator);
//IOIteratorNext(iterator); // skip interface #0
usbRef = IOIteratorNext(iterator);
IOObjectRelease(iterator);
IOCreatePlugInInterfaceForService(usbRef,
                                  kIOUSBInterfaceUserClientTypeID,
                                  kIOCFPlugInInterfaceID, &plugin, &score);
IOObjectRelease(usbRef);
(*plugin)->QueryInterface(plugin,
                          CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID300),
                          (LPVOID*)&usbInterface);
(*plugin)->Release(plugin);

ret = (*usbInterface)->USBInterfaceOpen(usbInterface);
if (ret != kIOReturnSuccess)
{
    printf("Could not open interface (error: %x)\n", ret);
    return false;
}
    device = usbDevice;
    interface = usbInterface;
    
    thread.Start();
    
    return true;
}

void BigButtonManager::Destroy() {
    if (interface) {
        IOUSBInterfaceInterface300** usbInterface = (IOUSBInterfaceInterface300**)interface;
        (*usbInterface)->USBInterfaceClose(usbInterface);
    }
    if (device) {
        IOUSBDeviceInterface300** usbDevice = (IOUSBDeviceInterface300**)device;
        (*usbDevice)->USBDeviceClose(usbDevice);
    }
    
    thread.Stop();
}

void callback(void *refcon, IOReturn result, void *arg0) {
    BigButtonManager* manager = (BigButtonManager*)arg0;
    manager->Print(result);
}

void BigButtonManager::Print(int result) {
    
    if (result == kIOReturnSuccess)
    {
        char whichPlayer = inputData[2];
        
        printf("inputData[0] = %d\n", (int)inputData[0]);
        printf("inputData[1] = %d\n", (int)inputData[1]);

        printf("player = %d\n", whichPlayer);

        printf("Back: %s \n", ((inputData[3] & 0x20) == 0x20) ? "down" : "up");
        printf("Start: %s \n", ((inputData[3] & 0x10) == 0x10) ? "down" : "up");
        printf("Right: %s \n", ((inputData[3] & 0x08) == 0x08) ? "down" : "up");
        printf("Left: %s \n", ((inputData[3] & 0x04) == 0x04) ? "down" : "up");
        printf("Down: %s \n", ((inputData[3] & 0x02) == 0x02) ? "down" : "up");
        printf("Up: %s \n", ((inputData[3] & 0x01) == 0x01) ? "down" : "up");
        
        printf("Y: %s \n", ((inputData[4] & 0x80) == 0x80) ? "down" : "up");
        printf("X: %s \n", ((inputData[4] & 0x40) == 0x40) ? "down" : "up");
        printf("B: %s \n", ((inputData[4] & 0x20) == 0x20) ? "down" : "up");
        printf("A: %s \n", ((inputData[4] & 0x10) == 0x10) ? "down" : "up");
        printf("BigButton: %s \n", ((inputData[4] & 0x08) == 0x08) ? "down" : "up");
        printf("Xbox: %s \n", ((inputData[4] & 0x04) == 0x04) ? "down" : "up");
        
        //printf("Read %d bytes\n", numBytes);
    }
    else
    {
        printf("Read failed (error: %x)\n", result);
    }

}

void BigButtonManager::Update(float dt) {
    if (!interface) return;
    for (int i=0; i<4; i++) {
        controller[i].Update(this, dt);
    }
}

void BigButtonManager::Controller::Update(BigButtonManager* manager, float dt) {

    //std::cout<<"Update"<<std::endl;
    bool hasChange = false;
    Thread::Lock();
    if (buttonsChanged) {
        hasChange = true;
        timeSinceButtonClick = 0;
        buttonsChanged = false;
        currentDowns = downs;
    }
    Thread::Unlock();
    if (!hasChange) {
        if (timeSinceButtonClick<0.15f) {
            timeSinceButtonClick+=dt;
            return;
        } else {
            Thread::Lock();
            downs.clear();
            Thread::Unlock();
            previousDowns = currentDowns;
            currentDowns.clear();
            timeSinceButtonClick = 0;
        }
    }

    for (ButtonDowns::iterator down = currentDowns.begin(); down!=currentDowns.end(); ++down) {
        ButtonDowns::iterator prev = previousDowns.find(*down);
        if (prev == previousDowns.end()) {
            manager->ButtonDown(*down);
        }
    }

    for (ButtonDowns::iterator up = previousDowns.begin(); up!=previousDowns.end(); ++up) {
        ButtonDowns::iterator cur = currentDowns.find(*up);
        if (cur == currentDowns.end()) {
            manager->ButtonUp(*up);
        }
    }

    previousDowns = currentDowns;
}

void BigButtonManager::PollLoop(Thread* thread) {

    IOUSBInterfaceInterface300** usbInterface = (IOUSBInterfaceInterface300**)interface;
    IOUSBInterfaceInterface300* inter = *usbInterface;
    
    while (true) {
        
        UInt32 numBytes = 5;
        for (int i=0; i<5; i++) {
            inputData[i] = 0;
        }
        
        IOReturn ret = inter->ReadPipe(usbInterface, 1, &inputData[0], &numBytes);

        if (ret != kIOReturnSuccess) {
            printf("Read failed (error: %x %x)\n", ret, kIOReturnNotReadable);
        } else {
            //Print(ret);
            UpdateButtonStates();
        }
    }
}

void BigButtonManager::UpdateButtonStates() {
    
    
    char playerIndex = inputData[2];
    
    /*
    
     printf("Back: %s \n", ((inputData[3] & 0x20) == 0x20) ? "down" : "up");
        printf("Start: %s \n", ((inputData[3] & 0x10) == 0x10) ? "down" : "up");
        printf("Right: %s \n", ((inputData[3] & 0x08) == 0x08) ? "down" : "up");
        printf("Left: %s \n", ((inputData[3] & 0x04) == 0x04) ? "down" : "up");
        printf("Down: %s \n", ((inputData[3] & 0x02) == 0x02) ? "down" : "up");
        printf("Up: %s \n", ((inputData[3] & 0x01) == 0x01) ? "down" : "up");
        
        printf("Y: %s \n", ((inputData[4] & 0x80) == 0x80) ? "down" : "up");
        printf("X: %s \n", ((inputData[4] & 0x40) == 0x40) ? "down" : "up");
        printf("B: %s \n", ((inputData[4] & 0x20) == 0x20) ? "down" : "up");
        printf("A: %s \n", ((inputData[4] & 0x10) == 0x10) ? "down" : "up");
        printf("BigButton: %s \n", ((inputData[4] & 0x08) == 0x08) ? "down" : "up");
        printf("Xbox: %s \n", ((inputData[4] & 0x04) == 0x04) ? "down" : "up");
    */
    
    char inputData3 = inputData[3];
    char inputData4 = inputData[4];
    
    Thread::Lock();
    
    controller[playerIndex].buttonsChanged = true;
    
    SetButtonDown(playerIndex, BigButtonButton::Back, (inputData3 & 0x20) == 0x20);
    SetButtonDown(playerIndex, BigButtonButton::Start, (inputData3 & 0x10) == 0x10);
    SetButtonDown(playerIndex, BigButtonButton::Right, (inputData3 & 0x08) == 0x08);
    SetButtonDown(playerIndex, BigButtonButton::Left, (inputData3 & 0x04) == 0x04);
    SetButtonDown(playerIndex, BigButtonButton::Down, (inputData3 & 0x02) == 0x02);
    SetButtonDown(playerIndex, BigButtonButton::Up, (inputData3 & 0x01) == 0x01);
    
    SetButtonDown(playerIndex, BigButtonButton::Y, (inputData4 & 0x80) == 0x80);
    SetButtonDown(playerIndex, BigButtonButton::X, (inputData4 & 0x40) == 0x40);
    SetButtonDown(playerIndex, BigButtonButton::B, (inputData4 & 0x20) == 0x20);
    SetButtonDown(playerIndex, BigButtonButton::A, (inputData4 & 0x10) == 0x10);
    SetButtonDown(playerIndex, BigButtonButton::BigButton, (inputData4 & 0x08) == 0x08);
    SetButtonDown(playerIndex, BigButtonButton::Xbox, (inputData4 & 0x04) == 0x04);
   
    Thread::Unlock();
}

void BigButtonManager::SetButtonDown(int playerIndex, BigButtonButton button, bool isDown) {
    BigButtonEvent e = {playerIndex, button };
    ButtonDowns& downs = controller[playerIndex].downs;
    if (isDown) {
        downs.insert(e);
    } else {
        ButtonDowns::iterator it = downs.find(e);
        if (it!=downs.end()) {
            downs.erase(it);
        }
    }
}
