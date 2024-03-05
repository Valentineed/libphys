#pragma once
#include <geometry/PxCapsuleGeometry.h>
#include "libphys_Export.h"

#include "Shapes/Shape.h"

class CapsuleShape : public Shape
{
public:
	libphys_API CapsuleShape(float radius, float halfHeight);
	PxCapsuleGeometry m_geometry;
};
