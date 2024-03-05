#include "SphereShape.h"

#include <PxPhysics.h>
#include "PhysixManager.h"
#include <stdexcept>

SphereShape::SphereShape(float radius)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_geometry.radius = radius;
	
	m_shape = physics->createShape(m_geometry, *m_material.m_material, false);
	if (!m_shape)
	{
		throw std::runtime_error("Fail create CapsuleShape");
	}
}
