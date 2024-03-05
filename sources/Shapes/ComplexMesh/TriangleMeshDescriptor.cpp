#include "TriangleMeshDescriptor.h"
//
//TriangleMeshDescriptor::TriangleMeshDescriptor(unsigned int triangleCount, const std::vector<PxU32>& indices,
//	unsigned int numbrVerts, const std::vector<PxVec3>& dataVerts)
//{
//	triangleDesc.points.count = static_cast<physx::PxU32>(numbrVerts);
//	triangleDesc.points.stride = sizeof(PxVec3);
//	triangleDesc.points.data = dataVerts.data();
//
//	triangleDesc.triangles.count = static_cast<physx::PxU32>(triangleCount);
//	triangleDesc.triangles.stride = 3 * sizeof(PxU32);
//	triangleDesc.triangles.data = indices.data();
//}

//TriangleMeshDescriptor::TriangleMeshDescriptor(std::pair<std::vector<uint32_t>&, std::vector<LibMath::Vector3>&>& dataModel)
//{
//	
//
//	triangleDesc.points.count = static_cast<physx::PxU32>(dataModel.second.size());
//	triangleDesc.points.stride = sizeof(PxVec3);
//	triangleDesc.points.data = dataModel.second.data();
//
//	triangleDesc.triangles.count = static_cast<physx::PxU32>(dataModel.first.size());
//	triangleDesc.triangles.stride = 3 * sizeof(PxU32);
//	triangleDesc.triangles.data = dataModel.first.data();
//
//}

TriangleMeshDescriptor::TriangleMeshDescriptor(std::vector<uint32_t>& indices, std::vector<LibMath::Vector3>& dataPos)
{
	triangleDesc.points.count = static_cast<physx::PxU32>(dataPos.size());
	triangleDesc.points.stride = sizeof(LibMath::Vector3);
	triangleDesc.points.data = &dataPos[0];

	triangleDesc.triangles.count = static_cast<physx::PxU32>(indices.size()/3);
	triangleDesc.triangles.stride = 3 * sizeof(uint32_t);
	triangleDesc.triangles.data = &indices[0];
}
