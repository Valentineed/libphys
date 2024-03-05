#include "BoxCollider.h"

#include <stdexcept>
#include <extensions/PxRigidActorExt.h>
#include <extensions/PxSimpleFactory.h>

#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "PhysixManager.h"
#include "PxScene.h"

BoxCollider::BoxCollider(float posX, float posY, float posZ, float xExtent, float yExtent, float zExtent)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_actor = physics->createRigidDynamic(PxTransform(posX, posY, posZ));
	if (!m_actor)
	{
		throw std::runtime_error("Fail create actor in BoxCollider");
	}

	auto test = BoxShape(xExtent, yExtent, zExtent);
	m_shape = test.m_shape;
	m_staticActor = PxCreateStatic(*physics, PxTransform(posX, posY, posZ), test.m_geometry, *m_material.m_material);
	//if (m_actor->attachShape(*m_shape) == false)
	//{
	//	throw std::runtime_error("Fail attach shape in actor BoxCollider");
	//}

	PhysixManager::GetInstance()->m_scene->addActor(*m_staticActor);
	//m_shape = PxRigidActorExt::createExclusiveShape(*m_actor, m_box,*m_material.m_material);
}

BoxCollider::~BoxCollider()
{
	m_shape->release();
}
