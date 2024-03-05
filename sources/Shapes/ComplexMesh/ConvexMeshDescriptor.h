#pragma once
#include <vector>
#include <cooking/PxConvexMeshDesc.h>
#include "libphys_Export.h"
#include "Vector/Vector3.h"

struct ConvexMeshDescriptor
{
	libphys_API ConvexMeshDescriptor() = default;
	libphys_API ConvexMeshDescriptor(unsigned int pointsCount, const std::vector<LibMath::Vector3>& dataVerts);
	physx::PxConvexMeshDesc convexDesc;
};
