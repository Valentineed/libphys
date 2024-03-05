#include "PhysixManager.h"

#include <iostream>
#include "Vector/Vector3.h"
#include "PxPhysicsAPI.h"

#include "Quaternion/Quaternion.h"
#include "cooking/PxCooking.h"
#include <PxFoundation.h>
#include <extensions\PxDefaultAllocator.h>
#include <PxPhysicsVersion.h>
#include <pvd\PxPvd.h>
#include <pvd\PxPvdTransport.h>
#include <PxPhysics.h>
#include <PxScene.h>
#include <PxVisualizationParameter.h>
#include <extensions\PxDefaultCpuDispatcher.h>
#include "PxSceneDesc.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "Debug/Debug.h"
#include "DefaultColliders/Primitives/PlaneCollider.h"
#include "DefaultColliders/Primitives/BoxCollider.h"
#include "PxPhysicsAPI.h"
#include <algorithm>
#include "Controller.h"
//#include "extensions/PxExtensionsAPI.h"
using namespace physx;

static ErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
PxFoundation* gFoundation;

void PhysixManager::Init(bool PVD, float objectLenght, float gravitySpeed)
{
	
	CreateFoundation();

	SetupToleranceScale(objectLenght, gravitySpeed);
	
	CreatePhysics(PVD);
	
	CreateCooking();
	void AddForceAtLocalPos();
	/*if(PVD)
	{
		if (!PxInitExtensions(*m_physics, m_pvd))
		{
			throw std::runtime_error("fail Init Extensions");
		}
	}*/

	CreateScene(PVD);

	m_cManager = PxCreateControllerManager(*m_scene);

	/*m_controller = new Controller(1.f, 0.5f, { -2,1,0});
	m_controller->ControllerJump(10);*/
	//m_plane = new PlaneCollider{0,1.f,0,100};
	//m_box = new BoxCollider{0,-1,0,0.2f,0.2f,0.2f};
	//Px Base -> Réflexion / interrogation des types d'objets.
	//PxActor -> Nom de l'acteur, indicateurs d'acteur, domination, clients, agrégats, limites du monde de la requête.
	//PxRigidActor -> Formes et transforme.
	//PxRigidBody -> Masse, inertie, vitesses, drapeaux corporels.
	//PxRigidStatic -> Interface pour corps statique dans la scène. Ce type de corps a une masse / inertie infinie implicite.
	//PxRigidDynamic -> Interface pour corps rigide dynamique dans la scène. Introduit la prise en charge des cibles cinématiques et des objets dormants.
	//PxArticulationLink -> Interface pour un lien de corps rigide dynamique dans une PxArticulation. Introduit la prise en charge de l'interrogation de l'articulation et des liens adjacents.
	//PxArticulation -> Définit l'interface pour une PxArticulation. Effectivement un contenu référençant plusieurs corps rigides PxArticualtionLink.
}

PhysixManager::~PhysixManager()
{
}

void PhysixManager::Update(float deltaTime)
{
	////https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/RigidBodyOverview.html
	m_accumulator += deltaTime;
	if(m_accumulator < m_stepSize)
	{
		return;
	}
	m_accumulator -= m_stepSize;
	m_scene->simulate(m_stepSize);

	m_scene->fetchResults(true);
}

PhysixManager* PhysixManager::GetInstance()
{
	static PhysixManager instance;

	return &instance;
}

void PhysixManager::End()
{
	m_physics->release();
}

void PhysixManager::SetGravity(const LibMath::Vector3& gravity) const
{
	m_scene->setGravity({gravity.x, gravity.y, gravity.z});
}


void PhysixManager::CreateFoundation()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	if (!m_foundation)
	{
		throw std::runtime_error("Error in creation Foundation");
	}
}

void PhysixManager::SetupToleranceScale(float objectLenght, float gravitySpeed)
{
	m_tolerancesScale.tolerancesScale.length = objectLenght;
	m_tolerancesScale.tolerancesScale.speed = gravitySpeed;
}

void PhysixManager::CreateScene(bool PVD)
{
	m_dispatcher = PxDefaultCpuDispatcherCreate(m_nbThreads);

	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.simulationEventCallback = &m_eventCallback;
	sceneDesc.filterShader = ContactReportFilterShader;
	m_scene = m_physics->createScene(sceneDesc);

	if(!m_scene)
	{
		throw std::runtime_error("Error in creation Scene Physix");
	}

	//m_scene->setSimulationEventCallback(&m_eventCallback);
	
	//Enable Debug Visualization
	m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
	
	if(PVD)
	{
		PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}
}

void PhysixManager::CreatePhysics(bool PVD)
{
	//The recordMemoryAllocations parameter specifies whether to perform memory profiling.
	bool  recordMemoryAllocations = true;
	int getErrorPVD = -1;
	if (PVD)
	{
		getErrorPVD = ConnectPVD();
	}
	if(getErrorPVD != -1)
	{
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation,
			m_tolerancesScale.tolerancesScale, recordMemoryAllocations, m_pvd);
	}
	else
	{
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation,
			m_tolerancesScale.tolerancesScale, recordMemoryAllocations);
	}
	
	if (!m_physics)
	{
		throw std::runtime_error("Error in creation Physics");
	}
}

void PhysixManager::CreateCooking()
{
	m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, PxCookingParams(m_tolerancesScale.tolerancesScale));
	if (!m_cooking)
	{
		throw std::runtime_error("PxCreateCooking failed!");
	}
}

int PhysixManager::ConnectPVD()
{
	m_pvd = PxCreatePvd(*m_foundation);
	m_transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10000);
	
	if(!m_transport)
	{
		std::cout << "transport fail in PVD" << std::endl;
		return -1;
	}
	
	m_pvd->connect(*m_transport, PxPvdInstrumentationFlag::eALL);
	return 0;
}

PxFilterFlags PhysixManager::ContactReportFilterShader(const PxFilterObjectAttributes attributes0, const PxFilterData filterData0, const PxFilterObjectAttributes attributes1, const PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, const PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_CONTACT_POINTS
		| PxPairFlag::eNOTIFY_TOUCH_LOST;
	return PxFilterFlag::eDEFAULT;
}
