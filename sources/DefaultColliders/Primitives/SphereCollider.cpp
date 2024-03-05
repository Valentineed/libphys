#include "SphereCollider.h"

#include <extensions/PxSimpleFactory.h>
#include "PhysixManager.h"
#include "PxRigidDynamic.h"

//#include "../../../libMaths/source/Vector/Vector3.h"

SphereCollider::SphereCollider(float x, float y, float z, float radius)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_transform = PxTransform{ x, y, z };
	PxReal density = 100;
	m_actor = PxCreateDynamic(*physics, m_transform, PxSphereGeometry(radius),
		*m_material.m_material, density);
}

SphereCollider::~SphereCollider()
{
	m_shape->release();
}
