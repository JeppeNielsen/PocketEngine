#pragma once
#include "GameComponent.hpp"
#include <btBulletDynamicsCommon.h>
#include "Vector3.hpp"
#include "Event.hpp"
#include <vector>

namespace Pocket {
	Component(RigidBody)
	public:

		RigidBody();
		
		btRigidBody::btRigidBodyConstructionInfo* Body;

		void ApplyImpulse(const Vector3& impulse, const Vector3& localPosition);

		struct ImpulseEvent {
			RigidBody* body;
			Vector3 impulse;
			Vector3 localPosition;
		};
        
        std::vector<ImpulseEvent> ImpulseEvents;

        void Reset();
        void Clone(const RigidBody& source);
        
	};
}
