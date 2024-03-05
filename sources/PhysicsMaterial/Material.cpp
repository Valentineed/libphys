#include "Material.h"

#include <stdexcept>

#include "../PhysixManager.h"
#include "PxPhysics.h"
Material::Material(float staticFriction, float dynamiqueFriction, float restitution)
{
	m_material = PhysixManager::GetInstance()->m_physics->createMaterial(staticFriction, dynamiqueFriction, restitution);
	if(!m_material)
	{
		throw std::runtime_error("Error in creation Physix Material");
	}
}
