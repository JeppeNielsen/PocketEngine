#pragma once
#include "GameSystem.hpp"
#include "RigidBody.hpp"
#include "Transform.hpp"
#include <map>
#include "Property.hpp"
#include <btBulletDynamicsCommon.h>
#include "GameWorld.hpp"


namespace Pocket {
	class PhysicsSystem : public GameSystem
	{
	public:
        
        PhysicsSystem();
        ~PhysicsSystem();
		
        void Initialize();

		void Update(float dt);

		Property<PhysicsSystem*, Vector3> Gravity;
		
	protected:
        
        
        
		void ObjectAdded(GameObject* gameObject);
		void ObjectRemoved(GameObject* gameObject);

	private:

		btCollisionConfiguration* collisionConfiguration;
		btDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btConstraintSolver* solver;
		btDynamicsWorld* physicsWorld;

		//btDefaultCollisionConfiguration m_collisionConfiguration;
		//btCollisionDispatcher* m_dispatcher;
		//btDbvtBroadphase* m_broadphase;
		//btSequentialImpulseConstraintSolver* m_solver;

		

		//typedef std::list<btMotionState*> MotionStates;
		//MotionStates motionStates;
		
		typedef std::map<GameObject*, btRigidBody*> Bodies;
		Bodies bodies;

		void TransformChanged(Transform* transform);

		bool isSimulating;

		void GravityChanged(PhysicsSystem* physicsSystem);

		void ImpulseReceived(RigidBody::ImpulseEvent& impulse, GameObject* object);

	};
}

