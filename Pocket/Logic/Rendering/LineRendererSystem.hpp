//
//  LineMeshSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 11/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "GameSystem.hpp"
#include "LineRenderer.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

namespace Pocket {
    class RenderSystem;
    class LineRendererSystem : public GameSystem<LineRenderer, Transform> {
    public:
        void Initialize() override;
        void Update(float dt) override;
        void Render() override;
    private:
        RenderSystem* renderSystem;
        GameStorage storage;
        GameWorld world;
        Mesh* lineMesh;
    };
}
