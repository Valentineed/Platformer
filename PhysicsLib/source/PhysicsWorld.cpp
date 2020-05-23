#include "PhysicsWorld.h"

#pragma warning(push)
#pragma warning(disable:4127)
#include "btBulletDynamicsCommon.h"
#pragma warning(pop)

#include <LinearMath/btAlignedObjectArray.h>

#include "RigidBody.h"

PhysicsWorld* PhysicsWorld::m_instance = nullptr;

PhysicsWorld::PhysicsWorld()
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_pCollisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    ///-----initialization_end-----
    m_instance = this;
}

PhysicsWorld::~PhysicsWorld()
{
   delete m_dynamicsWorld;
   delete m_solver;
   delete m_overlappingPairCache;
   delete m_dispatcher;
   delete m_pCollisionConfiguration;
}

PhysicsWorld* PhysicsWorld::GetInstance()
{
    if (m_instance == nullptr)
    {
        return  new PhysicsWorld();
    }

    return m_instance;
}

const btDefaultCollisionConfiguration* PhysicsWorld::GetCollisionConfigation() const
{
    return m_pCollisionConfiguration;
}

const btCollisionDispatcher* PhysicsWorld::GetDispatcher() const
{
    return m_dispatcher;
}

const btBroadphaseInterface* PhysicsWorld::GetOverLappingPairCache() const
{
    return m_overlappingPairCache;
}

const btSequentialImpulseConstraintSolver* PhysicsWorld::GetSolver() const
{
    return m_solver;
}

btDiscreteDynamicsWorld* PhysicsWorld::GetDynamicsWorld()
{
    return m_dynamicsWorld;
}

void PhysicsWorld::AddCollisionShape(btCollisionShape* shape)
{
    m_collisionShapes.push_back(shape);
}

void PhysicsWorld::RemoveRigidBody(btRigidBody* rigidBody)
{
    m_dynamicsWorld->removeRigidBody(rigidBody);
}

void PhysicsWorld::ClearCollisionShapes()
{
    m_collisionShapes.clear();
}



void PhysicsWorld::SetDebugDrawer(btIDebugDraw* debugDrawer)
{
    if (debugDrawer != nullptr)
    {
        m_dynamicsWorld->setDebugDrawer(debugDrawer);
    }
}

void PhysicsWorld::DrawWorld()
{
    m_dynamicsWorld->debugDrawWorld();
}

void PhysicsWorld::UpdateDynamicWorld(float deltaTime)
{
    m_dynamicsWorld->stepSimulation(deltaTime, 10);
}
