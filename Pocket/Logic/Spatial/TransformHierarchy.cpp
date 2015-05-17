#include "TransformHierarchy.hpp"

using namespace Pocket;

void TransformHierarchy::Initialize() {
	AddComponent<Transform>();
}

void TransformHierarchy::ObjectAdded(GameObject* gameObject) {
	Transform* transform = gameObject->GetComponent<Transform>();
	//transform->World.HasBecomeDirty += event_handler(this, &TransformHierarchy::WorldHasBecomeDirty);
	transform->World.Method += event_handler(this, &TransformHierarchy::CalcWorld, gameObject);
	gameObject->Parent.ChangedWithOld += event_handler(this, &TransformHierarchy::ParentChanged, gameObject);
	Property<GameObject*, GameObject*>::EventData e;
	e.owner = gameObject;
	e.Old = 0;
	e.Current = gameObject->Parent;
	ParentChanged(e, gameObject);
}

void TransformHierarchy::ObjectRemoved(GameObject* gameObject) {
	Transform* transform = gameObject->GetComponent<Transform>();
	//transform->World.HasBecomeDirty -= event_handler(this, &TransformHierarchy::WorldHasBecomeDirty);
	transform->World.Method -= event_handler(this, &TransformHierarchy::CalcWorld, gameObject);
	gameObject->Parent.ChangedWithOld -= event_handler(this, &TransformHierarchy::ParentChanged, gameObject);
}

void TransformHierarchy::CalcWorld( DirtyProperty<Transform*, Matrix4x4>::EventData& event, GameObject* object )
{
	Matrix4x4& world = *event.Value;
	if (object->Parent) {
		world = object->Parent()->GetComponent<Transform>()->World.GetValue()->Multiply(*event.owner->Local.GetValue());
	}
}

void TransformHierarchy::ParentChanged( Property<GameObject*, GameObject*>::EventData event, GameObject* object ) {
	Transform* transform = object->GetComponent<Transform>();
	transform->World.MakeDirty();
	transform->WorldInverse.MakeDirty();

	if (event.Old) {
        Transform* parentTransform = event.Old->GetComponent<Transform>();
        if (parentTransform) {
            transform->UnHookOther(parentTransform);
        }
    }
    
	if (event.Current) {
        Transform* parentTransform = event.Current->GetComponent<Transform>();
        if (parentTransform) {
            transform->HookOther(parentTransform);
        }
	}
}
