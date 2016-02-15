#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"

namespace Pocket {
    template<typename T>
	class TransformHierarchy : GameSystem<T, Transform> {
    
        using GameObject = GameObject<T>;
        
        void ObjectAdded(GameObject* gameObject) {
            Transform* transform = gameObject->template GetComponent<Transform>();
            //transform->World.HasBecomeDirty += event_handler(this, &WorldHasBecomeDirty);
            transform->World.Method += event_handler(this, &CalcWorld, gameObject);
            gameObject->Parent.ChangedWithOld += event_handler(this, &ParentChanged, gameObject);
            typename Property<GameObject*, GameObject*>::EventData e;
            e.owner = gameObject;
            e.Old = 0;
            e.Current = gameObject->Parent;
            ParentChanged(e, gameObject);
        }

        void ObjectRemoved(GameObject* gameObject) {
            Transform* transform = gameObject->template GetComponent<Transform>();
            //transform->World.HasBecomeDirty -= event_handler(this, &WorldHasBecomeDirty);
            transform->World.Method -= event_handler(this, &CalcWorld, gameObject);
            gameObject->Parent.ChangedWithOld -= event_handler(this, &ParentChanged, gameObject);
        }

        void CalcWorld( DirtyProperty<Transform*, Matrix4x4>::EventData& event, GameObject* object )
        {
            Matrix4x4& world = *event.Value;
            if (object->Parent) {
                Transform* parentTransform = object->Parent()->template GetComponent<Transform>();
                if (!parentTransform) {
                    world = *event.owner->Local.GetValue();
                } else {
                    world = parentTransform->World.GetValue()->Multiply(*event.owner->Local.GetValue());
                }
            }
        }

        void ParentChanged( typename Property<GameObject*, GameObject*>::EventData event, GameObject* object ) {
            Transform* transform = object->template GetComponent<Transform>();
            transform->World.MakeDirty();
            transform->WorldInverse.MakeDirty();

            if (event.Old) {
                Transform* parentTransform = event.Old->template GetComponent<Transform>();
                if (parentTransform) {
                    transform->UnHookOther(parentTransform);
                }
            }
            
            if (event.Current) {
                Transform* parentTransform = event.Current->template GetComponent<Transform>();
                if (parentTransform) {
                    transform->HookOther(parentTransform);
                }
            }
        }
    };
}
