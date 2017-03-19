//
//  DistanceScalerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include "DistanceScaler.hpp"
#include "Transform.hpp"

namespace Pocket {
    class RenderSystem;
    class DistanceScalerSystem : public GameSystem<DistanceScaler, Transform> {
    public:
        void Initialize();
        void Update(float dt);
        RenderSystem* renderSystem;
    };
}
