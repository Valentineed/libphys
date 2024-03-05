#pragma once
#include "ConfigPhysixTolerance.h"
#include "libphys_Export.h"
#include <RigidBody.h>
#include <vector>


#include "SimulationEventCallback.h"
#include "Actors/RigidType.h"
#include "PxPhysicsAPI.h"

namespace LibMath {
	struct Vector3;
}

struct PTransform;
class Shape;
struct ForceMode;
class BoxCollider;
class PlaneCollider;
class Controller;
using namespace physx;
//Forward declaration class in namespace https://stackoverflow.com/questions/19001700/how-to-forward-declare-a-class-which-is-in-a-namespace
namespace physx
{
	class PxShape;
	class PxPvdTransport;
	class PxCooking;
	class PxDefaultCpuDispatcher;
	class PxFoundation;
	class PxPvd;
	class PxTolerancesScale;
	class PxScene;
}

//https://documentation.help/NVIDIA-PhysX-SDK-Guide/ScenesAndActors.html
//https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/Geometry.html
class PhysixManager
{
public:
	
	void libphys_API Init(bool PVD = false, float objectLenght = 1, float gravitySpeed = 981);
	~PhysixManager();
	void libphys_API Update(float deltaTime);
	libphys_API static PhysixManager* GetInstance();
	void libphys_API End();

	//Set gravity of scene for all actors
	void libphys_API SetGravity(const LibMath::Vector3& gravity) const;

	
	//Set version PhysX and callbacks error
	PxFoundation* m_foundation;

	//PhysX Visual Debugger
    PxPvd* m_pvd;
    PxPvdTransport* m_transport;
	
	PxPhysics* m_physics;

	//Cooking library provides utilities for creating, converting, and serializing bulk data
    PxCooking* m_cooking;
	
    PxDefaultCpuDispatcher* m_dispatcher;
	PxScene* m_scene;
	PxControllerManager* m_cManager;

	TolerancesScale m_tolerancesScale;
    int m_nbThreads = 1;

	PlaneCollider* m_plane;
	BoxCollider* m_box;

	Controller* m_controller;
private:

	SimulationEventCallback m_eventCallback;
	
	float m_accumulator = 0.0f;
	float m_stepSize = 1.0f / 60.f;
	
	void CreateFoundation();
	void CreatePhysics(bool PVD);
	void CreateCooking();
	//Lenght = The approximate size of objects in the simulation.
	void SetupToleranceScale(float objectLenght, float gravitySpeed);

	void CreateScene(bool PVD);

	
	//PhysX Visual Debugger
	/*return -1 if fail*/
	int ConnectPVD();

	static PxFilterFlags ContactReportFilterShader(const PxFilterObjectAttributes attributes0,
		const PxFilterData filterData0,
		const PxFilterObjectAttributes attributes1,
		const PxFilterData filterData1, PxPairFlags& pairFlags,
		const void* constantBlock, const PxU32 constantBlockSize);
};