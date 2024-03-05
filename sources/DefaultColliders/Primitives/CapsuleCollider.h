#pragma once
#include "Collision.h"

class CapsuleCollider : public Collision
{
public:
	CapsuleCollider(float posX, float posY, float posZ, float radius, float halfHeight);
	~CapsuleCollider();
};
