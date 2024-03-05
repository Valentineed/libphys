#include "JointBase.h"


#include <iostream>
#include <stdexcept>


#include "PhysixManager.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
void Joint::CreateJoint(PxRigidActor* actor0, const PxTransform& tranform0, PxRigidActor* actor1, const PxTransform& tranform1)
{

	auto* physics = PhysixManager::GetInstance()->m_physics;
	m_joint = PxRevoluteJointCreate(*physics, actor0, tranform0, actor1,
		tranform1);
	if(!m_joint)
	{
		throw  std::runtime_error("fail create a static Actor");
	}
}

void Joint::SetBreakForce(float force, float torque) const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	m_joint->setBreakForce(force, torque);
}

void Joint::SetVisualization(bool isVisible) const
{
	if(!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	
	m_joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, isVisible);
}

bool Joint::IsBroken() const
{
	//TODO check if Work
	return (m_joint->getConstraintFlags() & PxConstraintFlag::eBROKEN);
}

void Joint::SetProjectionLinearTolerance(float tolerance) const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	m_joint->setProjectionLinearTolerance(tolerance);
}

void Joint::SetProjectionAngularTolerance(float tolerance) const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	m_joint->setProjectionAngularTolerance(tolerance);
}

void Joint::ActiveProjection(bool isActive) const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	m_joint->setConstraintFlag(PxConstraintFlag::ePROJECTION, isActive);
}

void Joint::SetLimit(float upper, float lower, float tolerance) const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	m_joint->setLimit(PxJointAngularLimitPair(lower, upper, tolerance));
}

void Joint::SetDefaultLimit() const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	PxJointAngularLimitPair limitPair(-PxPi / 4, PxPi / 4, 0.1f);
	limitPair.damping = 20.f;
	m_joint->setLimit(PxJointAngularLimitPair(-PxPi / 4, PxPi / 4, 0.1f));
}

void Joint::ActiveLimit(bool isActive) const
{
	if (!m_joint)
	{
		std::cerr << "the joint is not created";
		return;
	}
	m_joint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, isActive);
}
