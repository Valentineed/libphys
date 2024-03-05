#pragma once
#include <foundation/PxTransform.h>
#include "libphys_Export.h"

namespace LibMath {
	struct Quaternion;
	struct Vector3;
}

struct PTransform
{
	libphys_API PTransform() = default;
	libphys_API PTransform(float x, float y, float z, float qx, float qy, float qz, float qw);
	libphys_API	PTransform(float x, float y, float z);
	libphys_API PTransform(const LibMath::Vector3& pos);
	libphys_API PTransform(const LibMath::Vector3& pos, const LibMath::Quaternion& orientation);

	physx::PxTransform m_transform;
};
