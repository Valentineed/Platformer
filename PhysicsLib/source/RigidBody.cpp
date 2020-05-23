#include "RigidBody.h"
#include "Quaternion.h"
#include "Vec3.h"

#include "PhysicsWorld.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btScalar.h>



RigidBody::RigidBody(const Quaternion& rotation, const Vec3& position, const Vec3& boxShape, const float mass, Vec3 localInertia)
{
	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(boxShape.x(), boxShape.y(), boxShape.z()));

	m_boxshape = boxShape;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(position);
	startTransform.setRotation(rotation);

	bool isDynamic = (mass != 0.0f);

	if (isDynamic)
	{
		boxCollisionShape->calculateLocalInertia(mass, localInertia);
	}

	m_motionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,														// mass, in kg. 0 -> Static object, will never move.
		m_motionState,
		boxCollisionShape,										// collision shape of body
		localInertia												// local inertia
	);
	m_rigidBody = new btRigidBody(rigidBodyCI);

	PhysicsWorld::GetInstance()->AddCollisionShape(boxCollisionShape);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->addRigidBody(m_rigidBody);
}

RigidBody::RigidBody(const float radius, const Vec3& position, const float mass, Vec3 localInertia)
{
	btCollisionShape* sphereCollisionShape = new btSphereShape(radius);

	m_boxshape = { radius, radius, radius };

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(position);

	bool isDynamic = (mass != 0.0f);

	if (isDynamic)
	{
		sphereCollisionShape->calculateLocalInertia(mass, localInertia);
	}

	m_motionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,														// mass, in kg. 0 -> Static object, will never move.
		m_motionState,
		sphereCollisionShape,										// collision shape of body
		localInertia												// local inertia
	);
	m_rigidBody = new btRigidBody(rigidBodyCI);

	PhysicsWorld::GetInstance()->AddCollisionShape(sphereCollisionShape);
	PhysicsWorld::GetInstance()->GetDynamicsWorld()->addRigidBody(m_rigidBody);
}

float* RigidBody::GetTransform()
{
	btTransform transform = m_rigidBody->getWorldTransform();

	btScalar x, y, z;
	transform.getRotation().getEulerZYX(z, y, x);

	float* values = new float[6];
	values[0] = transform.getOrigin().getX();
	values[1] = transform.getOrigin().getY();
	values[2] = transform.getOrigin().getZ();
	values[3] = x;
	values[4] = y;
	values[5] = z;

	return values;
}

void RigidBody::Translate(Vec3 move)
{
	m_rigidBody->translate(move);
	m_rigidBody->activate(true);
}

void RigidBody::SetPosition(Vec3 pos)
{
	m_rigidBody->getWorldTransform().setOrigin(pos);
	m_rigidBody->activate(true);
}

void RigidBody::AddForce(Vec3 inertie)
{
	m_rigidBody->applyForce(inertie, m_rigidBody->getWorldTransform().getOrigin());
}

void RigidBody::FreezeRotation()
{
	m_rigidBody->setAngularFactor({ 0, 0, 0 });
}

void RigidBody::ChangeGravity(float multiplier)
{
	m_rigidBody->setGravity({ 0, PhysicsWorld::GetInstance()->GetDynamicsWorld()->getGravity().getY() * multiplier, 0 });
}

void RigidBody::RemoveRigidBody()
{
	PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
}

bool RigidBody::CheckSideCollisionWith(RigidBody* other)
{
	btVector3 rayFrom = m_rigidBody->getWorldTransform().getOrigin();
	float x = rayFrom.getX() - m_boxshape.getX() - 0.1f;
	btVector3 rayTo(x, rayFrom.getY(), rayFrom.getZ());

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
		{
			return false;
		}
		else if (pBody == other->m_rigidBody)
		{
			return true;
		}
	}

	////////////

	x = rayFrom.getX() + m_boxshape.getX() + 0.1f;
	btVector3 rayTo2(x, rayFrom.getY(), rayFrom.getZ());

	btCollisionWorld::ClosestRayResultCallback rayCallback2(rayFrom, rayTo2);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo2, rayCallback2);

	if (rayCallback2.hasHit())
	{
		btRigidBody* pBody2 = (btRigidBody*)btRigidBody::upcast(rayCallback2.m_collisionObject);
		if (!pBody2)
		{
			return false;
		}
		else if (pBody2 == other->m_rigidBody)
		{
			return true;
		}
	}

	///////////

	float z = rayFrom.getZ() - m_boxshape.getZ() - 0.1f;
	btVector3 rayTo3(rayFrom.getX(), rayFrom.getY(), z);

	btCollisionWorld::ClosestRayResultCallback rayCallback3(rayFrom, rayTo3);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo3, rayCallback3);

	if (rayCallback3.hasHit())
	{
		btRigidBody* pBody3 = (btRigidBody*)btRigidBody::upcast(rayCallback3.m_collisionObject);
		if (!pBody3)
		{
			return false;
		}
		else if (pBody3 == other->m_rigidBody)
		{
			return true;
		}
	}

	/////////////

	z = rayFrom.getZ() + m_boxshape.getZ() + 0.1f;
	btVector3 rayTo4(rayFrom.getX(), rayFrom.getY(), z);

	btCollisionWorld::ClosestRayResultCallback rayCallback4(rayFrom, rayTo4);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo4, rayCallback4);

	if (rayCallback4.hasHit())
	{
		btRigidBody* pBody4 = (btRigidBody*)btRigidBody::upcast(rayCallback4.m_collisionObject);
		if (!pBody4)
		{
			return false;
		}
		else if (pBody4 == other->m_rigidBody)
		{
			return true;
		}
	}

	return false;
}

bool RigidBody::CheckUpCollisionWith(RigidBody* other)
{
	btVector3 rayFrom = m_rigidBody->getWorldTransform().getOrigin();
	float y = rayFrom.getY() + m_boxshape.getY() + 0.1f;
	btVector3 rayTo(rayFrom.getX(), y, rayFrom.getZ());

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
		{
			return false;
		}
		else if (pBody == other->m_rigidBody)
		{
			return true;
		}
	}

	return false;
}

bool RigidBody::isGrounded()
{
	btVector3 rayFrom = m_rigidBody->getWorldTransform().getOrigin();
	float y = rayFrom.getY() - m_boxshape.getY() - 0.1f;
	btVector3 rayTo(rayFrom.getX(), y, rayFrom.getZ());

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
		{
			return false;
		}

		return true;
	}

	return false;
}

bool RigidBody::isGliding()
{

	btVector3 rayFrom = m_rigidBody->getWorldTransform().getOrigin();
	float x = rayFrom.getX() - m_boxshape.getX() - 0.1f;
	btVector3 rayTo(x, rayFrom.getY(), rayFrom.getZ());

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
		{
			return false;
		}

		return true;
	}

	////////////

	x = rayFrom.getX() + m_boxshape.getX() + 0.1f;
	btVector3 rayTo2(x, rayFrom.getY(), rayFrom.getZ());

	btCollisionWorld::ClosestRayResultCallback rayCallback2(rayFrom, rayTo2);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo2, rayCallback2);

	if (rayCallback2.hasHit())
	{
		btRigidBody* pBody2 = (btRigidBody*)btRigidBody::upcast(rayCallback2.m_collisionObject);
		if (!pBody2)
		{
			return false;
		}

		return true;
	}

	///////////

	float z = rayFrom.getZ() - m_boxshape.getZ() - 0.1f;
	btVector3 rayTo3(rayFrom.getX(), rayFrom.getY(), z);

	btCollisionWorld::ClosestRayResultCallback rayCallback3(rayFrom, rayTo3);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo3, rayCallback3);

	if (rayCallback3.hasHit())
	{
		btRigidBody* pBody3 = (btRigidBody*)btRigidBody::upcast(rayCallback3.m_collisionObject);
		if (!pBody3)
		{
			return false;
		}

		return true;
	}

	/////////////

	z = rayFrom.getZ() + m_boxshape.getZ() + 0.1f;
	btVector3 rayTo4(rayFrom.getX(), rayFrom.getY(), z);

	btCollisionWorld::ClosestRayResultCallback rayCallback4(rayFrom, rayTo4);

	PhysicsWorld::GetInstance()->GetDynamicsWorld()->rayTest(rayFrom, rayTo4, rayCallback4);

	if (rayCallback4.hasHit())
	{
		btRigidBody* pBody4 = (btRigidBody*)btRigidBody::upcast(rayCallback4.m_collisionObject);
		if (!pBody4)
		{
			return false;
		}

		return true;
	}

	return false;
	
}

RigidBody::~RigidBody()
{
	//TODO Delete a nullptr dont know why
	//delete m_motionState;
	//delete m_rigidBody;
}
