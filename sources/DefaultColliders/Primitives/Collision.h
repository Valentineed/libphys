#pragma once
#include <foundation/PxTransform.h>
#include "PhysicsMaterial/Material.h"

namespace physx {
	class PxRigidActor;
	class PxShape;
}

using namespace physx;
class Collision
{
protected:
	Collision() = default;
	virtual ~Collision() = default;
	PxShape* m_shape = nullptr;
	PxRigidActor* m_actor = nullptr;	                                            
	PxTransform m_transform;
	Material m_material;
};
