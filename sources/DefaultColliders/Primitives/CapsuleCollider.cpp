#include "CapsuleCollider.h"


#include <stdexcept>
#include <extensions/PxRigidBodyExt.h>


#include "PhysixManager.h"
#include "PxPhysics.h"
#include "PxRigidDynamic.h"
#include "PxScene.h"
#include "PxPhysicsAPI.h"

CapsuleCollider::CapsuleCollider(float posX, float posY, float posZ, float radius, float halfHeight)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_actor = physics->createRigidDynamic(PxTransform(posX,posY,posZ));
	if(!m_actor)
	{
		throw std::runtime_error("Fail create actor in CapsuleCollider");
	}
	
	m_shape = physics->createShape(PxCapsuleGeometry(radius, halfHeight), *m_material.m_material,false);
	if(!m_shape)
	{
		throw std::runtime_error("Fail create shape in CapsuleCollider");
	}
	if(m_actor->attachShape(*m_shape) == false)
	{
		throw std::runtime_error("Fail attach shape in actor CapsuleCollider");
	}
	
	PhysixManager::GetInstance()->m_scene->addActor(*m_actor);
}

CapsuleCollider::~CapsuleCollider()
{
	m_shape->release();
}
