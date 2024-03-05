#pragma once
namespace physx {
	class PxActor;
}

using namespace physx;
class Actor
{
public:
	void DisableGravity(bool isDisable);
	PxActor* m_actor;
};
