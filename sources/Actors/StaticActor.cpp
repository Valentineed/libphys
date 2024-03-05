#include "StaticActor.h"


#include <PxRigidStatic.h>
#include <stdexcept>

#include "PxPhysics.h"
#include "../PhysixManager.h"
StaticActor::StaticActor(const PxTransform& transform)
{
	auto* physics = PhysixManager::GetInstance()->m_physics;

	actor = physics->createRigidStatic(transform);

	
	if (!actor)
	{
		throw  std::runtime_error("fail create a static Actor");
	}
}

void StaticActor::AttachShape(PxShape* shape)
{
	if(!shape)
	{
		throw  std::runtime_error("fail attach shape in staticActor");
	}
	actor->attachShape(*shape);
}

void StaticActor::DetachShape(PxShape* shape)
{
	actor->detachShape(*shape);
}

void StaticActor::ReleaseActor()
{
	actor->release();
}
