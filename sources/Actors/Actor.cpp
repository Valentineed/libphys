#include "Actor.h"

#include "PxPhysicsAPI.h"
void Actor::DisableGravity(bool isDisable)
{
	m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, isDisable);
}
