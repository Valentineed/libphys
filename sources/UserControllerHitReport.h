#pragma once
#include <characterkinematic/PxController.h>

class UserControllerHitReport : public physx::PxUserControllerHitReport
{
public:
	void onShapeHit(const physx::PxControllerShapeHit& hit) override;
	void onControllerHit(const physx::PxControllersHit& hit) override;
	void onObstacleHit(const physx::PxControllerObstacleHit& hit) override;
};
