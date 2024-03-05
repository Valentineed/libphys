#pragma once
#include <foundation/PxSimpleTypes.h>

namespace physx {
	class PxMaterial;
}

using namespace physx;
class Material
{
public:
	Material(float staticFriction = PX_MAX_F32, float dynamiqueFriction = PX_MAX_F32, float restitution = 1.f);
	PxMaterial* m_material;
};
