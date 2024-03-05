#include "ConvexMeshDescriptor.h"

ConvexMeshDescriptor::ConvexMeshDescriptor(unsigned int pointsCount, const std::vector<LibMath::Vector3>& dataVerts)
{
	convexDesc.points.count = static_cast<physx::PxU32>(pointsCount);
	convexDesc.points.stride = sizeof(LibMath::Vector3);
	convexDesc.points.data = dataVerts.data();
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
}
