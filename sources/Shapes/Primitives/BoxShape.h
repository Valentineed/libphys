#pragma once
#include <geometry/PxBoxGeometry.h>
#include "libphys_Export.h"

#include "Shapes/Shape.h"

class BoxShape : public Shape
{
public:
	BoxShape() = default;
	libphys_API BoxShape(float xExtent, float yExtent, float zExtent);
	physx::PxBoxGeometry m_geometry;
};
