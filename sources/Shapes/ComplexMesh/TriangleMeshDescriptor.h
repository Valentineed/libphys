#pragma once
#include <vector>
#include <cooking/PxTriangleMeshDesc.h>
#include "libphys_Export.h"
#include "Vector/Vector3.h"
using namespace physx;
struct TriangleMeshDescriptor
{
	libphys_API TriangleMeshDescriptor() = default;
	libphys_API TriangleMeshDescriptor(std::vector<uint32_t>& indices, std::vector<LibMath::Vector3>& dataPos);
	PxTriangleMeshDesc triangleDesc;
};
