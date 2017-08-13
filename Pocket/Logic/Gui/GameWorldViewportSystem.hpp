//
//  GameWorldViewportSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "GameWorldViewport.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include "Mesh.hpp"
#include "TextureComponent.hpp"

namespace Pocket {
    class GameWorldViewportSystem : public GameSystem<GameWorldViewport, Transform, Sizeable, Mesh, TextureComponent> {
    protected:
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void Update(float dt) override;
        void Render() override;
    private:
        void UpdateObject(GameObject* object, float dt);
        void RenderObject(GameObject* object);
    };
}
