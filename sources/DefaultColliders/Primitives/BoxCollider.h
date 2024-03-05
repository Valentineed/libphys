#pragma once
#include <geometry/PxBoxGeometry.h>
#include "Collision.h"

#include "Shapes/Primitives/BoxShape.h"

namespace physx {
	class PxRigidStatic;
}

class BoxShape;
using namespace physx;
class BoxCollider : public Collision
{
public:
	BoxCollider(float posX, float posY, float posZ, float xExtent, float yExtent, float zExtent);
	~BoxCollider();
	PxRigidStatic* m_staticActor;
	
};
