#pragma once
#include <geometry/PxSphereGeometry.h>
#include "libphys_Export.h"

#include "Shapes/Shape.h"

class SphereShape : public Shape
{
public:
	libphys_API SphereShape(float radius);
	PxSphereGeometry m_geometry;
	
};
