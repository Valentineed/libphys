#include "ConvexMesh.h"



#include <PxRigidDynamic.h>
#include <stdexcept>
#include <extensions/PxDefaultStreams.h>

#include "ConvexMeshDescriptor.h"
#include "PhysixManager.h"
#include "cooking/PxCooking.h"
#include "PxPhysics.h"
#include "PxScene.h"

ConvexMesh::ConvexMesh(ConvexMeshDescriptor& descriptor)
{
	auto* cooking = PhysixManager::GetInstance()->m_cooking;
	auto* physics = PhysixManager::GetInstance()->m_physics;
	
	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	
	if(cooking->cookConvexMesh(descriptor.convexDesc, buf, &result) == false)
	{
		throw std::runtime_error("Error cook convex mesh");
	}
	
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	m_mesh = physics->createConvexMesh(input);
	if(!m_mesh)
	{
		throw std::runtime_error("Fail create a convex mesh");
	}
	m_shape = physics->createShape(PxConvexMeshGeometry(m_mesh), *m_material.m_material, false);
	
	if (!m_shape)
	{
		throw std::runtime_error("Fail create shape in convex mesh");
	}
}
