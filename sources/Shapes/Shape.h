#pragma once
#include "PhysicsMaterial/Material.h"
#include "libphys_Export.h"
namespace physx {
	class PxShape;
}

class Shape
{
public:
	libphys_API Shape() = default;
	virtual ~Shape() = default;
	
	physx::PxShape* m_shape = nullptr;
	Material m_material;
};
