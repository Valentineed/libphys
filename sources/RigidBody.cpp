#include "RigidBody.h"

#include "Forces/ForceMode.h"
#include "PxPhysicsAPI.h"

void RigidBody::AddForceAtPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode)
{
	PxRigidBodyExt::addForceAtPos(*m_body, force, pos, mode.myForce);
}

void RigidBody::AddForceAtLocalPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode)
{
	PxRigidBodyExt::addForceAtLocalPos(*m_body, force, pos, mode.myForce);
}

void RigidBody::AddLocalForceAtPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode)
{
	PxRigidBodyExt::addLocalForceAtPos(*m_body, force, pos, mode.myForce);
}

void RigidBody::AddLocalForceAtLocalPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode)
{
	PxRigidBodyExt::addLocalForceAtLocalPos(*m_body, force, pos, mode.myForce);
}

void RigidBody::SetLinearVelocity(const PxVec3& linVel)
{
	m_body->setLinearVelocity(linVel);
}

void RigidBody::SetAngularVelocity(const PxVec3& angVel)
{
	m_body->setAngularVelocity(angVel);
}
