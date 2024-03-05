#include "CapsuleShape.h"

#include <PxPhysics.h>
#include <stdexcept>



#include "PhysixManager.h"

CapsuleShape::CapsuleShape(float radius, float halfHeight)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_geometry.halfHeight = halfHeight;
	m_geometry.radius = radius;
	PxTransform  relativePose(PxQuat(PxHalfPi, { 0, 1, 0 }));
	m_shape = physics->createShape(m_geometry, *m_material.m_material, false);
	if (!m_shape)
	{
		throw std::runtime_error("Fail create CapsuleShape");
	}
	m_shape->setLocalPose(relativePose);
}
