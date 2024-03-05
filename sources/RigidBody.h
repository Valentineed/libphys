#pragma once
#include <PxRigidBody.h>

struct ForceMode;
using namespace physx;
class RigidBody
{
public:
	void AddForceAtPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode);
	void AddForceAtLocalPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode);
	void AddLocalForceAtPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode);
	void AddLocalForceAtLocalPos(const PxVec3& force, const PxVec3& pos, const ForceMode& mode);
	void SetLinearVelocity(const  PxVec3& linVel);
	void SetAngularVelocity(const  PxVec3& angVel);

	PxRigidBody* m_body;
};
