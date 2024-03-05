#pragma once
#include "RigidBody.h"

struct ForceMode;

namespace LibMath {
	struct Vector3;
}

namespace physx {
	class PxVec3;
	class PxShape;
	class PxTransform;
	class PxRigidDynamic;
}

using namespace physx;
using namespace LibMath;
struct DynamicActor : public RigidBody
{
	DynamicActor(const PxTransform& transform);

	[[nodiscard]] LibMath::Vector3 GetLinearVelocity();
	[[nodiscard]] LibMath::Vector3 GetAngularVelocity();

	void LockRotation(bool lockX, bool lockY, bool lockZ);
	void LockLocation(bool lockX, bool lockY, bool lockZ);
	
	void DisableGravity(bool disable);

	void SetMass(float mass);

	void SetMassSpaceInertiaTensor(const LibMath::Vector3& mass );
	
	void AttachShape(PxShape* shape);

	void DetachShape(PxShape* shape);
	
	void AddForce(const LibMath::Vector3& force, const ForceMode& mode, bool autowake = true);

	void AddForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true);
	void AddForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true);
	void AddLocalForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true);
	void AddLocalForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true);

	void AddTorque(const LibMath::Vector3& force, const ForceMode& mode, bool autowake = true);

	void SetAngularDamping(float angDamp);

	void SetLinearDamping(float linDamp);

	void SetKinematic(bool isKinematic);
	
	void ClearTorque(const ForceMode& mode);

	void ClearForce(const ForceMode& mode);

	void ReleaseActor();

	void PutToSleep();
	
	PxRigidDynamic* actor = nullptr;
};
