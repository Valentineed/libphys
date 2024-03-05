#include "DynamicActor.h"

#include <PxPhysics.h>
#include <PxRigidDynamic.h>
#include <stdexcept>
#include <extensions/PxRigidBodyExt.h>


#include "../Forces/ForceMode.h"

#include "PhysixManager.h"
#include "Vector/Vector3.h"

DynamicActor::DynamicActor(const PxTransform& transform)
{
	//auto* physics = PhysixManager::GetInstance()->m_physics;

	actor = PhysixManager::GetInstance()->m_physics->createRigidDynamic(transform);

	if(!actor)
	{
		throw  std::runtime_error("fail create a dynamic Actor");
	}
	
	m_body = actor;

}

LibMath::Vector3 DynamicActor::GetLinearVelocity()
{
	PxVec3 temp = actor->getLinearVelocity();
	return { temp.x, temp.y, temp.z };
}

LibMath::Vector3 DynamicActor::GetAngularVelocity()
{
	PxVec3 temp = actor->getAngularVelocity();
	return { temp.x, temp.y, temp.z };
}

void DynamicActor::LockRotation(bool lockX, bool lockY, bool lockZ)
{
	actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lockX);
	actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lockY);
	actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lockZ);
}

void DynamicActor::LockLocation(bool lockX, bool lockY, bool lockZ)
{
	actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, lockX);
	actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, lockY);
	actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, lockZ);
}

void DynamicActor::DisableGravity(bool disable)
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, disable);
}

void DynamicActor::SetMass(float mass)
{
	actor->setMass(mass);
}

void DynamicActor::SetMassSpaceInertiaTensor(const LibMath::Vector3& mass)
{
	actor->setMassSpaceInertiaTensor({ mass.x, mass.y,mass.z });
}

void DynamicActor::AttachShape(PxShape* shape)
{
	if(!shape)
	{
		throw  std::runtime_error("fail attach shape in dynamicActor");
	}
	actor->attachShape(*shape);
}

void DynamicActor::DetachShape(PxShape* shape)
{
	actor->detachShape(*shape);
}

void DynamicActor::AddForce(const LibMath::Vector3& force, const ForceMode& mode, bool autowake)
{
	actor->addForce({ force.x, force.y, force.z }, mode.myForce, autowake);
}

void DynamicActor::AddForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
	bool wakeup)
{
	PxRigidBodyExt::addForceAtPos(*actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, mode.myForce, wakeup);
}

void DynamicActor::AddForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
	bool wakeup)
{
	PxRigidBodyExt::addForceAtLocalPos(*actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, mode.myForce, wakeup);
}

void DynamicActor::AddLocalForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
	bool wakeup)
{
	PxRigidBodyExt::addLocalForceAtPos(*actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, mode.myForce, wakeup);
}

void DynamicActor::AddLocalForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos,
	const ForceMode& mode, bool wakeup)
{
	PxRigidBodyExt::addLocalForceAtLocalPos(*actor, { force.x, force.y, force.z }, { pos.x, pos.y, pos.z }, mode.myForce, wakeup);
}

void DynamicActor::AddTorque(const LibMath::Vector3& force, const ForceMode& mode, bool autowake)
{
	actor->addTorque({ force.x,force.y,force.z }, mode.myForce, autowake);
}

void DynamicActor::SetAngularDamping(float angDamp)
{
	actor->setAngularDamping(angDamp);
}

void DynamicActor::SetLinearDamping(float linDamp)
{
	actor->setLinearDamping(linDamp);
}

void DynamicActor::SetKinematic(bool isKinematic)
{
	actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}

void DynamicActor::ClearTorque(const ForceMode& mode)
{
	actor->clearForce(mode.myForce);
}

void DynamicActor::ClearForce(const ForceMode& mode)
{
	actor->clearForce(mode.myForce);
}

void DynamicActor::ReleaseActor()
{
	actor->release();
}

void DynamicActor::PutToSleep()
{
	actor->putToSleep();
}
