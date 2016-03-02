
#include "TransformHierarchy.hpp"

using namespace Pocket;

void TransformHierarchy::ObjectAdded(GameObject* object) {
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

void TransformHierarchy::ObjectRemoved(GameObject* object) {
    Transform* transform = object->template GetComponent<Transform>();
    transform->ResetWorldCalculation();
    object->Parent.Changed.Unbind(this, &TransformHierarchy::ParentChanged, object);
}

void TransformHierarchy::ParentChanged(GameObject* object) {
    Transform* transform = object->template GetComponent<Transform>();
    transform->World.MakeDirty();
    transform->WorldInverse.MakeDirty();

    if (object->Parent.PreviousValue()) {
        Transform* parentTransform = object->Parent.PreviousValue()->template GetComponent<Transform>();
        if (parentTransform) {
            transform->UnHookOther(parentTransform);
        }
    }
    
    HookParent(transform, object->Parent);
}

void TransformHierarchy::HookParent(Transform* transform, GameObject* parent) {
    if (parent) {
        Transform* parentTransform = parent->template GetComponent<Transform>();
        if (parentTransform) {
            transform->HookOther(parentTransform);
        }
    }
}

