//
//  HidDeviceSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 27/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "HidDevice.hpp"

namespace Pocket {
    class HidDeviceSystem : public GameSystem<HidDevice> {
    public:
        void Initialize();
        void Destroy();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        std::map<HidDevice*, void*> devices;
        void UpdateDevice(HidDevice* device);
    };
}
