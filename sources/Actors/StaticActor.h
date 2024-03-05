#pragma once
#include <foundation/PxTransform.h>

namespace physx {
	class PxShape;
	class PxRigidStatic;
}
using namespace physx;
struct StaticActor
{
	StaticActor(const PxTransform& transform);
	StaticActor() = default;
	void AttachShape(PxShape* shape);

	void DetachShape(PxShape* shape);
	void ReleaseActor();
	PxRigidStatic* actor;
	//physx::PxTransform transform;
};
