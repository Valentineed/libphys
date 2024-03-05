#include "PlaneCollider.h"



#include "PxRigidStatic.h"
#include <PxScene.h>
#include <stdexcept>
#include <extensions/PxSimpleFactory.h>

#include "PhysixManager.h"

PlaneCollider::PlaneCollider(float nx,float ny, float nz, float dist)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_shape = PxCreatePlane(*physics, PxPlane({ nx, ny, nz }, dist),
		*m_material.m_material );
	if(!m_shape)
	{
		throw std::runtime_error("Error in creation shape in Plane");
	}

	PhysixManager::GetInstance()->m_scene->addActor(*m_shape);
}

PlaneCollider::~PlaneCollider()
{
	m_shape->release();
}
