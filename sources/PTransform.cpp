#include "PTransform.h"


#include "Quaternion/Quaternion.h"
#include "Vector/Vector3.h"

PTransform::PTransform(float x, float y, float z, float qx, float qy, float qz, float qw)
{
	m_transform = physx::PxTransform{ x,y,z,{qx,qy,qz,qw} };
}

PTransform::PTransform(float x, float y, float z)
{
	m_transform = physx::PxTransform{ x,y,z };
}

PTransform::PTransform(const LibMath::Vector3& pos)
{
	m_transform = physx::PxTransform{ pos.x,pos.y,pos.z,{} };
}

PTransform::PTransform(const LibMath::Vector3& pos, const LibMath::Quaternion& orientation)
{
	m_transform = physx::PxTransform{ {pos.x,pos.y, pos.z },
		{orientation.X, orientation.Y, orientation.Z, orientation.W} };
}
