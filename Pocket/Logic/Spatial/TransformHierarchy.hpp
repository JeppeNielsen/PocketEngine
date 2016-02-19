#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"

namespace Pocket {
    template<typename T>
	class TransformHierarchy : public GameSystem<T, Transform> {
    public:
        using GameObject = GameObject<T>;
        
        void ObjectAdded(GameObject* object) {
            Transform* transform = object->template GetComponent<Transform>();
            
            transform->World.Method = [object, transform] (Matrix4x4& world) {
                if (object->Parent) {
                    Transform* parentTransform = object->Parent()->template GetComponent<Transform>();
                    if (!parentTransform) {
                        world = transform->Local();
                    } else {
                        world = parentTransform->World().Multiply(transform->Local);
                    }
                } else {
                    world = transform->Local;
                }
            };
        
            object->Parent.Changed.Bind(this, &TransformHierarchy::ParentChanged, object);
            HookParent(transform, object->Parent);
        }

        void ObjectRemoved(GameObject* object) {
            Transform* transform = object->template GetComponent<Transform>();
            transform->ResetWorldCalculation();
            object->Parent.Changed.Unbind(this, &TransformHierarchy::ParentChanged, object);
        }

        void ParentChanged(GameObject* &parent, GameObject* object ) {
            Transform* transform = object->template GetComponent<Transform>();
            transform->World.MakeDirty();
            transform->WorldInverse.MakeDirty();

            if (object->Parent.PreviousValue()) {
                Transform* parentTransform = object->Parent.PreviousValue()->template GetComponent<Transform>();
                if (parentTransform) {
                    transform->UnHookOther(parentTransform);
                }
            }
            
            HookParent(transform, parent);
        }
        
        void HookParent(Transform* transform, GameObject* parent) {
            if (parent) {
                Transform* parentTransform = parent->template GetComponent<Transform>();
                if (parentTransform) {
                    transform->HookOther(parentTransform);
                }
            }
        }
        
    };
}
