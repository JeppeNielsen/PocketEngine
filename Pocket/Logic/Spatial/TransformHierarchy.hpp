#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"

namespace Pocket {
	class TransformHierarchy : public GameSystem<Transform> {
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void ParentChanged(GameObject* object);
        void HookParent(Transform* transform, GameObject* parent);
    };
}
