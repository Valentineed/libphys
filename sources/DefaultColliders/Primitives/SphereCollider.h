#pragma once
#include "Collision.h"

namespace LibMath {
	struct Vector3;
}

class SphereCollider : Collision
{
public:
	SphereCollider(float x, float y, float z, float  radius);
	~SphereCollider();
};
