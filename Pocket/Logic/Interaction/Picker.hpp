//
//  Picker.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include <vector>
#include "Ray.hpp"
#include <functional>
#include "Vector3.hpp"
#include "Touchable.hpp"

namespace Pocket {
    class Picker {
        public:
            void TryAddClipper(GameObject* object);
            void TryRemoveClipper(GameObject* object);
            void Pick(GameObject* cameraObject, std::vector<TouchData>& list, const TouchEvent& e, bool forceClickThrough, const std::function<void(const Ray&, ObjectCollection& pickedObjects)>& octreeFunction, InputManager* input);
        
        private:
        
            std::vector<GameObject*> clippers;
        
            struct Intersection {
                GameObject* object;
                float distanceToPick;
                float u, v;
                size_t triIndex;
                Vector3 normal;
                Ray localRay;
            };
        
            bool SortClippers(GameObject *a, GameObject *b);
    
            GameObject* FindClipper(GameObject *fromThis);
            static bool SortIntersections(const Intersection &a, const Intersection &b);
    };
}
