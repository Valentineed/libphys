#pragma once
#include <characterkinematic/PxControllerBehavior.h>

class ControllerBehaviorCallback : public physx::PxControllerBehaviorCallback
{
public:
	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxController& controller) override;
	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxObstacle& obstacle) override;
	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxShape& shape, const physx::PxActor& actor) override;
};

