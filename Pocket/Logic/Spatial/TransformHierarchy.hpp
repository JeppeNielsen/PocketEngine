#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"

namespace Pocket {
	class TransformHierarchy : public GameSystem
	{
	public:
		 void Initialize();

	protected:

		void ObjectAdded(GameObject* gameObject);
		void ObjectRemoved(GameObject* gameObject);

	private:

		void WorldHasBecomeDirty(DirtyProperty<Transform*, Matrix4x4>* dirtyProperty);

		void CalcWorld(DirtyProperty<Transform*, Matrix4x4>::EventData& event, GameObject* object);

		void ParentChanged(Property<GameObject*, GameObject*>::EventData event, GameObject* object);

	};
}
