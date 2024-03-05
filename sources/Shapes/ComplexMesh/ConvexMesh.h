#pragma once
#include "libphys_Export.h"
#include "Shapes/Shape.h"

namespace physx {
	class PxConvexMesh;
}

struct ConvexMeshDescriptor;

class ConvexMesh : public Shape
{
public:
	libphys_API ConvexMesh(ConvexMeshDescriptor& descriptor);
	physx::PxConvexMesh* m_mesh;
};
