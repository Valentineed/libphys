#pragma once
#include "libphys_Export.h"
#include "PhysicsMaterial/Material.h"
#include "Shapes/Shape.h"

namespace physx {
	class PxRigidActor;
	class PxShape;
	class PxTriangleMesh;
}

struct TriangleMeshDescriptor;

//https://documentation.help/NVIDIA-PhysX-SDK-Guide/Shapes.html //TODO add Scalling ?
class TriangleMesh 
{
public:
	libphys_API TriangleMesh(TriangleMeshDescriptor& descriptor);
	PxTriangleMesh* m_mesh;
	Shape m_shape;
};
