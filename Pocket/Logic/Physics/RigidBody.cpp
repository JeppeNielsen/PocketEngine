#include "RigidBody.hpp"

using namespace Pocket;

RigidBody::RigidBody() {
	Body = 0;
}

void RigidBody::ApplyImpulse(const Vector3& impulse, const Vector3& localPosition) {
	ImpulseEvent impulseEvent;
	impulseEvent.body = this;
	impulseEvent.impulse = impulse;
	impulseEvent.localPosition = localPosition;
	ImpulseEvents.push_back(impulseEvent);
}
