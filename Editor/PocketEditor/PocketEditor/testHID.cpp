#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"
#include <unistd.h>

#include "GameWorld.hpp"

#include "HidDeviceSystem.hpp"
#include "CompetitionProJoystickSystem.hpp"

#define MAX_STR 255

using namespace Pocket;

int main_testHid(int argc, char* argv[])
{
    
    GameWorld world;
    GameObject* root = world.CreateRoot();
    root->CreateSystem<HidDeviceSystem>();
    root->CreateSystem<CompetitionProJoystickSystem>();
    
    GameObject* joyStickGo = root->CreateObject();
    auto device = joyStickGo->AddComponent<HidDevice>();
    
    //competition pro
    device->VendorId = 0x040b;
    device->ProductId = 0x6533;
    
    //xbox controller
    //device->VendorId = 0x045e;
    //device->ProductId = 0x028e;
    
    //big button controller
    //device->VendorId = 0x045e;
    //device->ProductId = 0x02a0;
    
    auto joyStick = joyStickGo->AddComponent<CompetitionProJoystick>();

    joyStick->ButtonDown.Bind([] (int index) {
        std::cout << "Button down"<< std::endl;
    });
    
    joyStick->ButtonUp.Bind([] (int index) {
        std::cout << "Button up"<< std::endl;
    });

    while (true) {
        world.Update(1.0f);
    
        //std::cout << "loop"<<std::endl;
        usleep(1000);
    }

    int res;
    unsigned char buf[65];
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    int i;

    // Initialize the hidapi library
    res = hid_init();

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    
    handle = hid_open(0x040b, 0x6533, NULL);

    //  Product ID:    0xc52f
    //  Vendor ID:    0x046d

    //handle = hid_open(0x046d, 0xc52f, NULL);

    // Read the Manufacturer String
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    wprintf(L"Manufacturer String: %s\n", wstr);

    // Read the Product String
    res = hid_get_product_string(handle, wstr, MAX_STR);
    wprintf(L"Product String: %s\n", wstr);

    // Read the Serial Number String
    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

    // Read Indexed String 1
    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    wprintf(L"Indexed String 1: %s\n", wstr);

/*
    // Toggle LED (cmd 0x80). The first byte is the report number (0x0).
    buf[0] = 0x0;
    buf[1] = 0x80;
    res = hid_write(handle, buf, 65);

    

    // Request state (cmd 0x81). The first byte is the report number (0x0).
    buf[0] = 0x0;
    buf[1] = 0x81;
    res = hid_write(handle, buf, 65);
*/
    while (true) {
    
    
    // Read requested state
    res = hid_read(handle, buf, 65);

    // Print out the returned buffer.
    for (i = 0; i < 4; i++)
        printf("buf[%d]: %d\n", i, buf[i]);
    
        usleep(1000);
    }

    // Finalize the hidapi library
    res = hid_exit();

    return 0;
}
