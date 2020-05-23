#pragma once

#include "Vec3.h"
#include "Quaternion.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btDefaultMotionState.h>

class RigidBody
{
public:
	RigidBody() = delete;
	RigidBody(const RigidBody& other) = delete;
	RigidBody& operator=(RigidBody& other) = delete;
	~RigidBody();

	RigidBody(const btQuaternion& rotation, const btVector3& position, const btVector3& boxShape, const float mass, btVector3 localInertia); // boxShape
	RigidBody(const btScalar radius, const btVector3& position, const float mass, btVector3 localInertia); // spherShape
	
	float* GetTransform();

	void Translate(Vec3 move);
	void SetPosition(Vec3 pos);
	void AddForce(Vec3 inertie);
	void FreezeRotation();
	void ChangeGravity(float multiplier);
	void RemoveRigidBody();


	bool CheckSideCollisionWith(RigidBody* other);
	bool CheckUpCollisionWith(RigidBody* other);



	bool isGrounded();
	bool isGliding();

private:

	

	btRigidBody* m_rigidBody = nullptr;
	btDefaultMotionState* m_motionState = nullptr;

	int m_collisionGroup = 0;
	int m_collisionMask = 0;
	btVector3 m_boxshape;
};