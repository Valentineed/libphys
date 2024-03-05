#include "SimulationEventCallback.h"


#include <iostream>
#include <PxRigidActor.h>
#include "PhysixCollider.h"
#include "PhysixManager.h"

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs,
                                        PxU32 nbPairs)
{
	for (PxU32 pairIndex = 0; pairIndex < nbPairs; pairIndex++)
	{
		const  PxContactPair& contactPair = pairs[pairIndex];

		if(contactPair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND) &&
			!contactPair.flags.isSet(PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH))
		{
			auto collider = static_cast<Physix::PhysixCollider*>(pairHeader.actors[0]->userData);
			auto otherCollider = static_cast<Physix::PhysixCollider*>(pairHeader.actors[1]->userData);

			if(otherCollider != nullptr)
			{
				collider->OnContact(otherCollider->m_idEntity);
			}
		}
		
		if (contactPair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST))
		{
			auto collider = static_cast<Physix::PhysixCollider*>(pairHeader.actors[0]->userData);

			auto otherCollider = static_cast<Physix::PhysixCollider*>(pairHeader.actors[1]->userData);
			if (otherCollider != nullptr)
			{
				collider->EndContact(otherCollider->m_idEntity);
			}
		}
		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
			return;
	}
}

void SimulationEventCallback::onAdvance(const PxRigidBody* const* /*bodyBuffer*/, const PxTransform* /*poseBuffer*/, const PxU32 /*count*/)
{
	std::cout << "OnAdvance" << std::endl;
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
{
	std::cout << "OnContraint" << std::endl;
}

void SimulationEventCallback::onSleep(PxActor** /*actors*/, PxU32 /*count*/)
{

	std::cout << "OnSleep" << std::endl;
}

void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 /*count*/)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND) 
	{
		auto collider = static_cast<Physix::PhysixCollider*>(pairs->triggerActor->userData);
		auto otherCollider = static_cast<Physix::PhysixCollider*>(pairs->otherActor->userData);

		collider->OnContact(otherCollider->m_idEntity);
	}

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		auto collider = static_cast<Physix::PhysixCollider*>(pairs->triggerActor->userData);
		auto otherCollider = static_cast<Physix::PhysixCollider*>(pairs->otherActor->userData);;

		collider->EndContact(otherCollider->m_idEntity);
	}
}

void SimulationEventCallback::onWake(PxActor** /*actors*/, PxU32 /*count*/)
{
	std::cout << "OnWake" << std::endl;
}
