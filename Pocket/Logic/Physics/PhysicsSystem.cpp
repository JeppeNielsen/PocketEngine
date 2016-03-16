#include "PhysicsSystem.hpp"


using namespace Pocket;

PhysicsSystem::PhysicsSystem() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	physicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
		
	isSimulating = false;

	Gravity.Changed.Bind(this, &PhysicsSystem::GravityChanged);
	Gravity = Vector3(0,-10,0);
}

PhysicsSystem::~PhysicsSystem() {
	delete physicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;
}

void PhysicsSystem::Update(float dt) {

    for (Bodies::iterator it = bodies.begin(); it!=bodies.end(); ++it) {
        RigidBody* rigidBody = it->first->GetComponent<RigidBody>();
        if (rigidBody->ImpulseEvents.empty()) continue;
        btRigidBody* body = it->second;
        for (int i=0; i<rigidBody->ImpulseEvents.size(); i++) {
            RigidBody::ImpulseEvent impulse = rigidBody->ImpulseEvents[i];
            body->applyImpulse(btVector3(impulse.impulse.x, impulse.impulse.y, impulse.impulse.z),
                               btVector3(impulse.localPosition.x, impulse.localPosition.y, impulse.localPosition.z));
        }
        rigidBody->ImpulseEvents.clear();
    }
    
	physicsWorld->stepSimulation(dt * 10);

	isSimulating = true;
	for (Bodies::iterator it = bodies.begin(); it!=bodies.end(); ++it) {
        btRigidBody* body = it->second;
		Transform* transform = (Transform*)body->getUserPointer();
		btTransform world;
		body->getMotionState()->getWorldTransform(world);
		btVector3 position = world.getOrigin();
		btQuaternion rotation = world.getRotation();
		transform->Position = Vector3(position.x(),position.y(),position.z());
		transform->Rotation = Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z());
	}
	isSimulating = false;
	
}

void PhysicsSystem::ObjectAdded(GameObject* gameObject) {
	RigidBody* rigidBody = gameObject->GetComponent<RigidBody>();
	if (!rigidBody->Body) return;
	Transform* transform = gameObject->GetComponent<Transform>();
	
	const Matrix4x4& world = transform->World;
	Vector3 position = world.Translation();
	Quaternion rotation;
	rotation.FromRotationMatrix(world);
	btTransform startTransform;
	startTransform.setOrigin(btVector3(position.x,position.y,position.z));
	startTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(*rigidBody->Body);
	rbInfo.m_motionState = motionState;

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setUserPointer(transform);
	//motionStates.push_back(motionState);
	bodies[gameObject] = body;
	
	this->physicsWorld->addRigidBody(body);

	//transform->World.Changed += event_handler(this, &PhysicsSystem::TransformChanged);
}

void PhysicsSystem::ObjectRemoved(GameObject* gameObject) {
	RigidBody* rigidBody = gameObject->GetComponent<RigidBody>();
	if (!rigidBody->Body) return;
	//Transform* transform = gameObject->GetComponent<Transform>();
    Bodies::iterator it = bodies.find(gameObject);
	btRigidBody* body = it->second;
	bodies.erase(it);
	//motionStates.remove(body->getMotionState());
	this->physicsWorld->removeRigidBody(body);
	delete body->getMotionState();
	delete body;
	//transform->Transform.Changed -= event_handler(this, &PhysicsSystem::TransformChanged);
}


void PhysicsSystem::ImpulseReceived(RigidBody::ImpulseEvent& impulse, GameObject* object) {
	btRigidBody* body = bodies[object];
	body->applyImpulse(btVector3(impulse.impulse.x, impulse.impulse.y, impulse.impulse.z),
		btVector3(impulse.localPosition.x, impulse.localPosition.y, impulse.localPosition.z));
}

void PhysicsSystem::GravityChanged() {
	const Vector3& gravity = Gravity;
	physicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}