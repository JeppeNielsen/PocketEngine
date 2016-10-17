//
//  GameObject.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Pocket {
    class GameObject {
    public:
        template<typename T>
        T* GetComponent() {
            return new T();
        }
    };
}