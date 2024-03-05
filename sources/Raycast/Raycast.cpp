#include "Raycast.h"


#include "PhysixCollider.h"
#include "PhysixManager.h"
#include "Vector/Vector3.h"

bool Raycast::Ray(LibMath::Vector3 origin, LibMath::Vector3 dir, float maxDistance, HitInfo& hitInfo)
{
	bool result = PhysixManager::GetInstance()->m_scene->raycast({ origin.x, origin.y, origin.z },
		{ dir.x, dir.y, dir.z }, maxDistance, hitInfo.GetBuffer());

	return result;
}

float HitInfo::GetDistance() const
{
	return buffer.block.distance;
}

LibMath::Vector3 HitInfo::GetPosition() const
{
	PxVec3 pos = buffer.block.position;
	return { pos.x, pos.y,pos.z};
}

LibMath::Vector3 HitInfo::GetNormal() const
{
	PxVec3 norm = buffer.block.normal;
	return { norm.x, norm.y,norm.z };
}

size_t HitInfo::GetEntityID() const
{
	auto collider = static_cast<Physix::PhysixCollider*>(buffer.block.actor->userData);
	return collider->m_idEntity;
}

PxRaycastBuffer& HitInfo::GetBuffer()
{
	return buffer;
}
