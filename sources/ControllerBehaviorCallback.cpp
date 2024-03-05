#include "ControllerBehaviorCallback.h"

physx::PxControllerBehaviorFlags ControllerBehaviorCallback::getBehaviorFlags(
	const physx::PxController& /*controller*/)
{

	return physx::PxControllerBehaviorFlags(0);
}

physx::PxControllerBehaviorFlags ControllerBehaviorCallback::getBehaviorFlags(const physx::PxObstacle& /*obstacle*/)
{
	return physx::PxControllerBehaviorFlags(0);
}

physx::PxControllerBehaviorFlags ControllerBehaviorCallback::getBehaviorFlags(const physx::PxShape& /*shape*/,
	const physx::PxActor& /*actor*/)
{
	return physx::PxControllerBehaviorFlags(0);
}