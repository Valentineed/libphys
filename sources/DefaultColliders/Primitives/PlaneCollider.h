#pragma once
#include <foundation/PxTransform.h>

#include "PhysicsMaterial/Material.h"

namespace physx {
	class PxRigidStatic;
}
using namespace physx;
class PlaneCollider
{
public:
	PlaneCollider(float nx, float ny, float nz, float dist);
	~PlaneCollider();
	PxRigidStatic* m_shape;
	PxTransform m_transform;
	Material m_material;
};
