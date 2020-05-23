#pragma once

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
#pragma warning(push)
#pragma warning(disable:4127)
#include "btBulletDynamicsCommon.h"
#pragma warning(pop)

class  mat4;

template<typename T = btCollisionShape*>
class btAlignedObjectArray;


class PhysicsWorld
{
public:
	static PhysicsWorld* GetInstance();

	const btDefaultCollisionConfiguration* GetCollisionConfigation() const;
	const btCollisionDispatcher* GetDispatcher() const;
	const btBroadphaseInterface* GetOverLappingPairCache() const;
	const btSequentialImpulseConstraintSolver* GetSolver() const;

	btDiscreteDynamicsWorld* GetDynamicsWorld();
	void AddCollisionShape(btCollisionShape* shape);

	void RemoveRigidBody(btRigidBody* rigidBody);
	void ClearCollisionShapes();


	void SetDebugDrawer(btIDebugDraw* debugDrawer);
	void DrawWorld();

	void UpdateDynamicWorld(float deltaTime);
		
private:
	PhysicsWorld();
	~PhysicsWorld();
	static PhysicsWorld* m_instance;

	btDefaultCollisionConfiguration* m_pCollisionConfiguration = nullptr;
	btCollisionDispatcher* m_dispatcher = nullptr;
	btBroadphaseInterface* m_overlappingPairCache = nullptr;
	btSequentialImpulseConstraintSolver* m_solver = nullptr;
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
};