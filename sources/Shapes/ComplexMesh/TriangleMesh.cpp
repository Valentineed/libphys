#include "TriangleMesh.h"

#include <stdexcept>
#include <extensions/PxDefaultStreams.h>



#include "PhysixManager.h"
#include "PxPhysics.h"
#include "PxRigidDynamic.h"
#include "cooking/PxCooking.h"
#include "TriangleMeshDescriptor.h"
#include "PxScene.h"
#include "PxPhysicsAPI.h"

TriangleMesh::TriangleMesh(TriangleMeshDescriptor& descriptor)
{
	auto* cooking = PhysixManager::GetInstance()->m_cooking;
	auto* physics = PhysixManager::GetInstance()->m_physics;
	
	const PxTolerancesScale scale;
	PxCookingParams params(scale);
	// disable mesh cleaning - perform mesh validation on development configurations
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	// disable edge precompute, edges are set for each triangle, slows contact generation
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	
	cooking->setParams(params);
	
	m_mesh = cooking->createTriangleMesh(descriptor.triangleDesc, physics->getPhysicsInsertionCallback());
	/*PxDefaultMemoryOutputStream buf;
	PxTriangleMeshCookingResult::Enum result;
	if (!cooking->cookTriangleMesh(descriptor.triangleDesc, buf, &result))
	{
		throw std::runtime_error("Error cook convex mesh");
	}
	
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	m_mesh = physics->createTriangleMesh(input);
	
	if (!m_mesh)
	{
		throw std::runtime_error("Fail create a convex mesh");
	}*/
}
