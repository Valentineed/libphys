#include "BoxShape.h"


#include <PxPhysics.h>
#include <stdexcept>


#include "PhysixManager.h"

BoxShape::BoxShape(float xExtent, float yExtent, float zExtent)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_geometry.halfExtents = { xExtent,yExtent,zExtent };
	m_shape = physics->createShape(m_geometry, *m_material.m_material, false);
	if (!m_shape)
	{
		throw std::runtime_error("Fail create shape in CapsuleCollider");
	}
}
