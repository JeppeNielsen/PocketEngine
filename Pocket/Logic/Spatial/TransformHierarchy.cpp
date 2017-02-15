
#include "TransformHierarchy.hpp"

using namespace Pocket;

void TransformHierarchy::ObjectAdded(GameObject* object) {

    GameObject* ownerObject = object->GetComponentOwner<Transform>();
    if (ownerObject != object) return;
    
    Transform* transform = object->GetComponent<Transform>();
    
    transform->World.Method = [object, transform] (Matrix4x4& world) {
        if (object->Parent()) {
            Transform* parentTransform = object->Parent()->GetComponent<Transform>();
            if (!parentTransform) {
                world = transform->Local();
            } else {
                world = parentTransform->World().Multiply(transform->Local);
            }
        } else {
            world = transform->Local;
        }
    };
    
    transform->World.MakeDirty();
    transform->WorldInverse.MakeDirty();
    object->Parent.Changed.Bind(this, &TransformHierarchy::ParentChanged, object);
    HookParent(transform, object->Parent());
}

void TransformHierarchy::ObjectRemoved(GameObject* object) {

    GameObject* ownerObject = object->GetComponentOwner<Transform>();
    if (ownerObject != object) return;

    Transform* transform = object->GetComponent<Transform>();
    transform->ResetWorldCalculation();
    object->Parent.Changed.Unbind(this, &TransformHierarchy::ParentChanged, object);
    if (object->Parent()) {
        Transform* parentTransform = object->Parent()->GetComponent<Transform>();
        if (parentTransform) {
            transform->UnHookOther(parentTransform);
        }
    }
}

void TransformHierarchy::ParentChanged(GameObject* object) {
    Transform* transform = object->GetComponent<Transform>();
    
    /*
    Matrix4x4 prevWorld = transform->World;
    Vector3 scaleX = prevWorld.TransformVector({1,0,0});
    Vector3 scaleY = prevWorld.TransformVector({0,1,0});
    Vector3 scaleZ = prevWorld.TransformVector({0,0,1});
    */
    
    const Matrix4x4 oldWorld = transform->World;
    
    transform->World.MakeDirty();
    transform->WorldInverse.MakeDirty();
    
    Transform* oldParentTransform = object->Parent.PreviousValue() ?
    object->Parent.PreviousValue()->GetComponent<Transform>()
    :
    0;

    if (oldParentTransform) {
        transform->UnHookOther(oldParentTransform);
    }
    
    Transform* newParentTransform = object->Parent() ?
    object->Parent()->GetComponent<Transform>()
    :
    0;
    
    if (newParentTransform) {
        transform->HookOther(newParentTransform);
    }

    const Matrix4x4& newWorld = !newParentTransform ?
        Matrix4x4::IDENTITY
        :
        newParentTransform->WorldInverse;

    Matrix4x4 intoNewWorld = newWorld * oldWorld;
    
    transform->Position = intoNewWorld.TransformPosition(0);
    transform->Scale = {
        intoNewWorld.TransformVector({1,0,0}).Length(),
        intoNewWorld.TransformVector({0,1,0}).Length(),
        intoNewWorld.TransformVector({0,0,1}).Length()
    };
    

    Vector3 x = { intoNewWorld.m[0][0], intoNewWorld.m[1][0], intoNewWorld.m[2][0] };
    Vector3 y = { intoNewWorld.m[0][1], intoNewWorld.m[1][1], intoNewWorld.m[2][1] };
    Vector3 z = { intoNewWorld.m[0][2], intoNewWorld.m[1][2], intoNewWorld.m[2][2] };
    x.Normalize();
    y.Normalize();
    z.Normalize();
    
    intoNewWorld.m[0][0] = x.x;
    intoNewWorld.m[1][0] = x.y;
    intoNewWorld.m[2][0] = x.z;
    
    intoNewWorld.m[0][1] = y.x;
    intoNewWorld.m[1][1] = y.y;
    intoNewWorld.m[2][1] = y.z;
    
    intoNewWorld.m[0][2] = z.x;
    intoNewWorld.m[1][2] = z.y;
    intoNewWorld.m[2][2] = z.z;
    
    Quaternion quat;
    quat.FromRotationMatrix(intoNewWorld);
    transform->Rotation = quat;
}

void TransformHierarchy::HookParent(Transform* transform, GameObject* parent) {
    if (parent) {
        Transform* parentTransform = parent->GetComponent<Transform>();
        if (parentTransform) {
            transform->HookOther(parentTransform);
        }
    }
}

