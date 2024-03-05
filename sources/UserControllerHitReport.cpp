#include "UserControllerHitReport.h"


#include <iostream>

void UserControllerHitReport::onShapeHit(const physx::PxControllerShapeHit& /*hit*/)
{
	std::cout << "OntHit" << std::endl;
}

void UserControllerHitReport::onControllerHit(const physx::PxControllersHit& /*hit*/)
{
	std::cout << "onControllerHit" << std::endl;
}

void UserControllerHitReport::onObstacleHit(const physx::PxControllerObstacleHit& /*hit*/)
{
	std::cout << "onObstacleHit" << std::endl;
}
