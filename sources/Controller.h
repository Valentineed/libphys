#pragma once
#include "ControllerBehaviorCallback.h"
#include "PhysixManager.h"
#include "PxPhysicsAPI.h"
#include "UserControllerHitReport.h"
#include "PhysicsMaterial/Material.h"
#include "Vector/Vector3.h"
#include <iostream>
using namespace physx;

static PxF32 gJumpGravity = -50.0f;
class Jump
{
public:
	Jump()
	{
		mV0 = (0.0f);
		mJumpTime = (0.0f);
		mJump = (false);
	}

	PxF32		mV0;
	PxF32		mJumpTime;
	bool		mJump;

	void		startJump(PxF32 v0);
	void		stopJump();
	PxF32		getHeight(PxF32 elapsedTime);
};

void Jump::startJump(PxF32 v0)
{
	if (mJump)	return;
	mJumpTime = 0.0f;
	mV0 = v0;
	mJump = true;
}

void Jump::stopJump()
{
	if (!mJump)	return;
	mJump = false;
	//mJumpTime = 0.0f;
	//mV0	= 0.0f;
}

PxF32 Jump::getHeight(PxF32 elapsedTime)
{
	if (!mJump)	return 0.0f;
	mJumpTime += elapsedTime;
	const PxF32 h = gJumpGravity * mJumpTime * mJumpTime + mV0 * mJumpTime;
	return h * elapsedTime;
}

class Controller : public PxQueryFilterCallback
{
public:
	Controller(float height, float radius, const LibMath::Vector3& pos)
	{
		m_statingSize = height;
		physx::PxCapsuleControllerDesc desc;
		desc.material = m_material.m_material;
		desc.position = {pos.x, pos.y,pos.z};
		desc.height = m_statingSize;
		desc.radius = radius;
		desc.slopeLimit = 0.0f;
		desc.contactOffset = 0.1f;
		desc.stepOffset = 0.02f;
		desc.reportCallback = &m_controllerHit;
		desc.behaviorCallback = &m_callbackBehavior;
		
		m_controller = static_cast<PxCapsuleController*>(PhysixManager::GetInstance()->m_cManager->createController(desc));
		// remove controller shape from scene query for standup overlap test
		PxRigidDynamic* actor = m_controller->getActor();
		if (actor)
		{
			if (actor->getNbShapes())
			{
				PxShape* ctrlShape;
				actor->getShapes(&ctrlShape, 1);
				ctrlShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
			}
			else
				std::cout << "character actor has no shape"<< std::endl;
		}
		else
			std::cout << "character could not create actor" << std::endl;
		
		mFilterCallback = this;

	}
	~Controller()
	{
		m_controller->release();
	}
	PxQueryHitType::Enum			preFilter(const PxFilterData& /*filterData*/, const PxShape* /*shape*/, const PxRigidActor* /*actor*/, PxHitFlags& /*queryFlags*/)override
	{
		return PxQueryHitType::eTOUCH;
	}
	PxQueryHitType::Enum postFilter(const PxFilterData& /*filterData*/, const PxQueryHit& /*hit*/) override
	{
		return PxQueryHitType::eTOUCH;
	}
	LibMath::Vector3 GetPosition()
	{
		auto pos = m_controller->getPosition();
		return { (float)pos.x, (float)pos.y, (float)pos.z };
	}
	LibMath::Vector3 GetFootPosition()
	{
		auto pos = m_controller->getFootPosition();
		return { (float)pos.x, (float)pos.y, (float)pos.z };
	}
	LibMath::Vector3 GetVectorUp()
	{
		auto pos = m_controller->getUpDirection();
		return { pos.x, pos.y, pos.z };
	}
	void SetVectorUp(const LibMath::Vector3& up)
	{
		m_controller->setUpDirection({ up.x, up.y, up.z });
	}

	void ControllerJump(float vel)
	{
		m_jump.startJump(vel);
	}
	
	void Update(float DeltaTime)
	{
		//const PxControllerFilters filters(m_filterData, m_filterCallback, m_filterCallbackController);

		const PxControllerFilters filters(nullptr, mFilterCallback, nullptr);
		PxVec3 disp;
		
		const PxF32 heightDelta = m_jump.getHeight(DeltaTime);
		float dy;
		if (heightDelta != 0.0f)
			dy = heightDelta;
		else
			dy = m_gravity * DeltaTime;
		disp = m_movDir;
		disp.y = dy;
		if(m_controller)
		{
			const PxU32 flags = m_controller->move(disp, 0.001f, DeltaTime, PxControllerFilters(), nullptr);//TODO TEST 
			if (flags & PxControllerCollisionFlag::eCOLLISION_DOWN)
			{
				m_jump.stopJump();
			}
			
		}
	}
	void Move(const LibMath::Vector3& direction)
	{
		m_movDir = { direction.x, direction.y, direction.z };
	}
	
	void SetOverlapRecoveryModule(bool tf)
	{
		PhysixManager::GetInstance()->m_cManager->setOverlapRecoveryModule(tf);
	}
	
	void TryStandup()
	{
		PxSceneWriteLock scopedLock(*PhysixManager::GetInstance()->m_scene);

		// overlap with upper part
		PxReal r = m_controller->getRadius();
		PxReal dh = m_statingSize - m_crouchingSize - 2 * r;
		PxCapsuleGeometry geom(r, dh * .5f);

		PxExtendedVec3 position = m_controller->getPosition();
		PxVec3 pos((float)position.x, (float)position.y + m_statingSize * .5f + r, (float)position.z);
		PxQuat orientation(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));

		PxOverlapBuffer hit;
		if (PhysixManager::GetInstance()->m_scene->overlap(geom, PxTransform(pos, orientation), hit,
			PxQueryFilterData(PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC)))
			return;

		// if no hit, we can stand up
		SetHeight(m_statingSize);
	}

	void Crounching(float height)
	{
		m_crouchingSize = height;
		SetHeight(height);
	}
	float m_statingSize;
	float m_crouchingSize;
	
private:

	void SetHeight(float height)
	{
		PxSceneWriteLock scopedLock(*PhysixManager::GetInstance()->m_scene);
		m_controller->resize(height);
	}
	Jump m_jump;
	PxCapsuleController* m_controller;
	Material m_material;
	float m_gravity = -9.81f;
	PxVec3 m_movDir = { 0,0,0 };

	bool m_bfirstT = true;
	float m_elapsedTime = 0;
	ControllerBehaviorCallback m_callbackBehavior;
	UserControllerHitReport m_controllerHit;
	/*const PxFilterData* m_filterData;
	PxQueryFilterCallback* m_filterCallback;
	PxControllerFilterCallback* m_filterCallbackController;*/
	PxControllerFilters gCharacterControllerFilters;
	const PxFilterData* mFilterData;		// User-defined filter data for 'move' function
	PxQueryFilterCallback* mFilterCallback;	// User-defined filter data for 'move' function
	PxControllerFilterCallback* mCCTFilterCallback;	// User-defined filter data for 'move' function
};
