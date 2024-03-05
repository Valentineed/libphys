#include "PhysixCollider.h"


#include <iostream>
#include <stdexcept>
#include <extensions/PxRigidActorExt.h>

#include <PxScene.h>
#include <stdexcept>
#include <extensions/PxSimpleFactory.h>

#include "PxPhysicsAPI.h"
#include "PhysixManager.h"
#include "PTransform.h"
#include "Actors/DynamicActor.h"
#include "Shapes/Shape.h"
#include "Actors/StaticActor.h"
#include "PxRigidActor.h"
#include "PxRigidStatic.h"//keep this for the dynamic cast
#include "PxRigidDynamic.h"//keep this for the dynamic cast
#include "Shapes/ComplexMesh/TriangleMesh.h"
#include "Shapes/Shape.h"
#include "Vector/Vector3.h"
#include "PxPhysicsAPI.h"
#include "Quaternion/Quaternion.h"
#include "Shapes/Primitives/BoxShape.h"
#include "Shapes/Primitives/CapsuleShape.h"
#include "Shapes/Primitives/SphereShape.h"

namespace Physix
{
	PhysixCollider::PhysixCollider(Shape* shape, RigidType type, const PTransform& transform, size_t entityID)
	{
		if(!shape)
		{
			throw std::runtime_error("Fail shape is null in Collider");
		}
		m_shape = shape;
		m_type = type;
		m_transform = transform;
		m_idEntity = entityID;

		switch (type)
		{
		case RigidType::DYNAMIC:
			m_dynamic = new DynamicActor(transform.m_transform);
			m_dynamic->AttachShape(shape->m_shape);
			m_dynamic->actor->userData = this;
			PhysixManager::GetInstance()->m_scene->addActor(*m_dynamic->actor);
			break;
		case RigidType::STATIC:
			m_static = new StaticActor(transform.m_transform);
			m_static->AttachShape(shape->m_shape);
			m_static->actor->userData = this;
			PhysixManager::GetInstance()->m_scene->addActor(*m_static->actor);
			break;
		}	
	}

	PhysixCollider::PhysixCollider(TriangleMesh& triangleMesh, RigidType type, const PTransform& transform, size_t entityID)
	{
		m_shape = &triangleMesh.m_shape;
		m_type = type;
		m_transform = transform;
		m_idEntity = entityID;
		
		switch (type)
		{
		case RigidType::DYNAMIC:
			m_dynamic = new DynamicActor(transform.m_transform);
			m_dynamic->SetKinematic(true);
			triangleMesh.m_shape.m_shape = PxRigidActorExt::createExclusiveShape(*m_dynamic->actor, 
				PxTriangleMeshGeometry(triangleMesh.m_mesh),
				*triangleMesh.m_shape.m_material.m_material);
			
			
			m_dynamic->AttachShape(triangleMesh.m_shape.m_shape);
			m_dynamic->actor->userData = this;
			PhysixManager::GetInstance()->m_scene->addActor(*m_dynamic->actor);
			break;
		case RigidType::STATIC:
			m_static = new StaticActor(transform.m_transform);
			triangleMesh.m_shape.m_shape = PhysixManager::GetInstance()->m_physics->createShape(
				PxTriangleMeshGeometry(triangleMesh.m_mesh), *triangleMesh.m_shape.m_material.m_material, false);

			m_static->AttachShape(triangleMesh.m_shape.m_shape);
			m_static->actor->userData = this;
			PhysixManager::GetInstance()->m_scene->addActor(*m_static->actor);
			break;
		}
	}

	PhysixCollider::PhysixCollider(float nx, float ny, float nz, float dist, const PTransform& transform, size_t entityID)
	{
		auto* physics = PhysixManager::GetInstance()->m_physics;

		m_type = RigidType::PLANE;
		m_transform = transform;
		m_idEntity = entityID;
		m_static = new StaticActor();
		m_static->actor = PxCreatePlane(*physics, PxPlane({ nx, ny, nz }, dist),
			*m_material.m_material);
		m_static->actor->userData = this;
		if (!m_static->actor)
		{
			throw std::runtime_error("Error in creation shape in Plane");
		}

		PhysixManager::GetInstance()->m_scene->addActor(*m_static->actor);
	}

	PhysixCollider::PhysixCollider(PhysixCollider&& other) noexcept
	{
		this->m_shape = other.m_shape;
		this->m_dynamic = other.m_dynamic;
		this->m_idEntity = other.m_idEntity;
		if(m_dynamic)
		{
			this->m_dynamic->actor = other.m_dynamic->actor;
			this->m_dynamic->actor->userData = this;
		}
		this->m_static = other.m_static;
		if(m_static)
		{
			this->m_static->actor = other.m_static->actor;
			this->m_static->actor->userData = this;
		}
		this->m_transform = other.m_transform;
		this->m_type = other.m_type;
		this->m_onBeginContactDelegate = other.m_onBeginContactDelegate;
		this->m_onEndContactDelegate = other.m_onEndContactDelegate;
		this->bSetBeginContact = other.bSetBeginContact;
		this->bSetEndContact = other.bSetEndContact;

		other.m_shape = nullptr;
		other.m_dynamic = nullptr;
		other.m_static = nullptr;
	}

	PhysixCollider& PhysixCollider::operator=(PhysixCollider&& other)
	{
		this->m_shape = other.m_shape;
		this->m_dynamic = other.m_dynamic;
		this->m_idEntity = other.m_idEntity;
		if (m_dynamic)
		{
			this->m_dynamic->actor = other.m_dynamic->actor;
			this->m_dynamic->actor->userData = this;
		}
		this->m_static = other.m_static;
		if (m_static)
		{
			this->m_static->actor = other.m_static->actor;
			this->m_static->actor->userData = this;
		}
		this->m_transform = other.m_transform;
		this->m_type = other.m_type;
		this->m_onBeginContactDelegate = other.m_onBeginContactDelegate;
		this->m_onEndContactDelegate = other.m_onEndContactDelegate;
		this->bSetBeginContact = other.bSetBeginContact;
		this->bSetEndContact = other.bSetEndContact;

		other.m_shape = nullptr;
		other.m_dynamic = nullptr;
		other.m_static = nullptr;
		return *this;
	}

	PhysixCollider::~PhysixCollider()
	{
		delete m_dynamic;
		delete m_static;
	}

	bool PhysixCollider::operator==(const PhysixCollider& rhs) const
	{
		if(m_static && rhs.m_static)
		{
			return this->m_static->actor == rhs.m_static->actor;
		}
		if(m_dynamic && rhs.m_dynamic)
		{
			return this->m_dynamic->actor == rhs.m_dynamic->actor;
		}
		return false;
	}

	void PhysixCollider::ChangeType(RigidType type)
	{
		if(m_type == type)
		{
			return;
		}
		
		switch (type)
		{
		case RigidType::STATIC:
			m_dynamic->DetachShape(m_shape->m_shape);
			m_static = new StaticActor(m_transform.m_transform);
			m_static->AttachShape(m_shape->m_shape);
			m_static->actor->userData = this;
			PhysixManager::GetInstance()->m_scene->addActor(*m_static->actor);
			m_dynamic->ReleaseActor();
			delete m_dynamic;
			m_dynamic = nullptr;
			break;
		case RigidType::DYNAMIC:
			m_static->DetachShape(m_shape->m_shape);
			m_dynamic = new DynamicActor(m_transform.m_transform);
			m_dynamic->AttachShape(m_shape->m_shape);
			m_dynamic->actor->userData = this;
			PhysixManager::GetInstance()->m_scene->addActor(*m_dynamic->actor);
			m_static->ReleaseActor();
			delete m_static;
			m_static = nullptr;
			break;
		}
		m_type = type;
	}

	void PhysixCollider::SetKinematic(bool isKinematic) const
	{
		if(m_type == RigidType::STATIC)
		{
			return;
		}
		m_dynamic->SetKinematic(isKinematic);
	}

	void PhysixCollider::Release()
	{
		switch (m_type)
		{
		case RigidType::STATIC:
			m_static->actor->release();
			m_shape->m_shape->release();
			break;
		case RigidType::DYNAMIC:
			m_dynamic->actor->release();
			m_shape->m_shape->release();
			break;
		case RigidType::PLANE:
			m_static->actor->release();
			break;
		default://because remove warning (no sense....)
			break;
		}
	}

	LibMath::Vector3 PhysixCollider::GetLinearVelocity() const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return Vector3::Zero;
		}
		return m_dynamic->GetLinearVelocity();
	}

	LibMath::Vector3 PhysixCollider::GetAngularVelocity() const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return Vector3::Zero;
		}
		return m_dynamic->GetAngularVelocity();
	}

	float PhysixCollider::GetMass() const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return 0.0f;
		}
		return m_dynamic->actor->getMass();
	}
	
	
	void libphys_API PhysixCollider::SetScale(LibMath::Vector3& scale)
	{
		if (m_type == RigidType::PLANE)
		{
			return;
		}

		const Vector3 newScale = scale * 0.5f;
		switch (m_type)
		{
		case RigidType::STATIC:
			m_static->DetachShape(m_shape->m_shape);
			switch (m_shape->m_shape->getGeometry().getType())
			{
			case PxGeometryType::eBOX:
				m_shape->m_shape->release();
				m_shape = new BoxShape(newScale.x, newScale.y, newScale.z);
				break;
			case PxGeometryType::eCAPSULE:
				m_shape->m_shape->release();
				m_shape = new CapsuleShape(scale.x, scale.y);
				break;
			case PxGeometryType::eSPHERE:
				m_shape->m_shape->release();
				m_shape = new SphereShape(scale.x);
				break;
			}
			//Change Scale
			m_static->AttachShape(m_shape->m_shape);
			break;
		case RigidType::DYNAMIC:
			m_dynamic->DetachShape(m_shape->m_shape);
			switch (m_shape->m_shape->getGeometry().getType())
			{
			case PxGeometryType::eBOX:
				m_shape->m_shape->release();
				m_shape = new BoxShape(newScale.x, newScale.y, newScale.z);
				break;
			case PxGeometryType::eCAPSULE:
				m_shape->m_shape->release();
				m_shape = new CapsuleShape(scale.x, scale.y);
				break;
			case PxGeometryType::eSPHERE:
				m_shape->m_shape->release();
				m_shape = new SphereShape(scale.x);
				break;
			}
			m_dynamic->AttachShape(m_shape->m_shape);
			break;
		}
		
	}

	void PhysixCollider::SetLinearVelocity(const LibMath::Vector3& linVel) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->SetLinearVelocity({ linVel.x, linVel.y, linVel.z  });
	}

	void PhysixCollider::SetAngularVelocity(const LibMath::Vector3& angVel) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->SetAngularVelocity({ angVel.x, angVel.y, angVel.z });
	}

	void PhysixCollider::DisableGravity(bool disable) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->DisableGravity(disable);
	}

	void PhysixCollider::SetMass(float mass) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->SetMass(mass);
	}

	void PhysixCollider::SetMassSpaceInertiaTensor(const LibMath::Vector3& mass) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->SetMassSpaceInertiaTensor(mass);
	}

	void PhysixCollider::AddForce(const LibMath::Vector3& force, const ForceMode& mode) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->AddForce(force, mode);
	}

	void PhysixCollider::AddTorque(const LibMath::Vector3& force, const ForceMode& mode) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->AddTorque(force, mode);
	}

	void PhysixCollider::SetAngularDamping(float angDamp) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->SetAngularDamping(angDamp);
	}

	void PhysixCollider::SetLinearDamping(float linDamp) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->SetLinearDamping(linDamp);
	}

	void PhysixCollider::ClearTorque(const ForceMode& mode) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->ClearTorque(mode);
	}

	void PhysixCollider::ClearForce(const ForceMode& mode) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->ClearForce(mode);
	}

	void PhysixCollider::AddForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
		bool wakeup) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->AddForceAtPos(force, pos, mode, wakeup);
	}

	void PhysixCollider::AddForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
		bool wakeup) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->AddForceAtLocalPos(force, pos, mode, wakeup);
	}

	void PhysixCollider::AddLocalForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
		bool wakeup) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->AddLocalForceAtPos(force, pos, mode, wakeup);
	}

	void PhysixCollider::AddLocalForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos,
		const ForceMode& mode, bool wakeup) const
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->AddLocalForceAtLocalPos(force, pos, mode, wakeup);
	}

	void PhysixCollider::OnContact(size_t indexOtherActor)
	{
  		if(bSetBeginContact)
		{
  			m_onBeginContactDelegate(indexOtherActor);
		}
	}

	void PhysixCollider::EndContact(size_t indexOtherActor)
	{
		if(bSetEndContact)
		{
			m_onEndContactDelegate(indexOtherActor);
		}
	}

	void PhysixCollider::SetName(const char* name) const
	{
		switch (m_type)
		{
		case RigidType::STATIC:
			m_static->actor->setName(name);
			break;
		case RigidType::PLANE:
			m_static->actor->setName(name);
			break;
		case RigidType::DYNAMIC:
			m_dynamic->actor->setName(name);
			break;
		default://because remove warning (no sense....)
			break;
		}
	}

	const char* PhysixCollider::GetName() const
	{
		switch (m_type)
		{
		case RigidType::STATIC:
			return m_static->actor->getName();
		case RigidType::PLANE:
			return m_static->actor->getName();
		case RigidType::DYNAMIC:
			return m_dynamic->actor->getName();
		default://because remove warning (no sense....)
			return "null";
		}
	}

	physx::PxRigidActor* PhysixCollider::GetActor() const
	{
		switch (m_type)
		{
		case RigidType::STATIC:
			return static_cast<physx::PxRigidActor*>(m_static->actor);
		case RigidType::PLANE:
			return static_cast<physx::PxRigidActor*>(m_static->actor);
		case RigidType::DYNAMIC:
			return static_cast<physx::PxRigidActor*>(m_dynamic->actor);
			default://because remove warning (no sense....)
				return nullptr;
		}
	}

	void PhysixCollider::PutToSleep()
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->PutToSleep();
	}

	void PhysixCollider::LockRotation(bool lockX, bool lockY, bool lockZ)
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->LockRotation(lockX, lockY, lockZ);
	}

	void PhysixCollider::LockLocation(bool lockX, bool lockY, bool lockZ)
	{
		if (m_type == RigidType::STATIC || m_type == RigidType::PLANE)
		{
			return;
		}
		m_dynamic->LockLocation(lockX, lockY, lockZ);
	}

	void libphys_API PhysixCollider::ShapeInShapePairIntersectionTests(bool active)
	{
		if (m_shape)
		{
			switch (m_type)
			{
			case RigidType::STATIC:
				m_static->actor->detachShape(*m_shape->m_shape);
				m_shape->m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, active);
				m_static->actor->attachShape(*m_shape->m_shape);
				break;
			case RigidType::DYNAMIC:
				m_dynamic->actor->detachShape(*m_shape->m_shape);
				m_shape->m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, active);
				m_dynamic->actor->attachShape(*m_shape->m_shape);
				break;
			default://because remove warning (no sense....)
				break;
			}
		}
	}

	void libphys_API PhysixCollider::ShapeInSceneQueryTests(bool active)
	{
		if (m_shape)
		{
			switch (m_type)
			{
			case RigidType::STATIC:
				m_static->actor->detachShape(*m_shape->m_shape);
				m_shape->m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, active);
				m_static->actor->attachShape(*m_shape->m_shape);
				break;
			case RigidType::DYNAMIC:
				m_dynamic->actor->detachShape(*m_shape->m_shape);
				m_shape->m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, active);
				m_dynamic->actor->attachShape(*m_shape->m_shape);
				break;
			default://because remove warning (no sense....)
				break;
			}
		}
	}

	void PhysixCollider::TriggerCollision(bool bisTrigger) const
	{
		if (m_shape)
		{
			switch (m_type)
			{
			case RigidType::STATIC:
				m_static->actor->detachShape(*m_shape->m_shape);
				if (bisTrigger == false)
				{
					m_shape->m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, bisTrigger);
					m_shape->m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !bisTrigger);
				}
				else
				{
					m_shape->m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !bisTrigger);
					m_shape->m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, bisTrigger);
				}
				m_static->actor->attachShape(*m_shape->m_shape);
				break;
			case RigidType::DYNAMIC:
				m_dynamic->actor->detachShape(*m_shape->m_shape);
				if (bisTrigger == false)
				{
					m_shape->m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, bisTrigger);
					m_shape->m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !bisTrigger);
				}
				else
				{
					m_shape->m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !bisTrigger);
					m_shape->m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, bisTrigger);
				}
			
				m_dynamic->actor->attachShape(*m_shape->m_shape);
				break;
			default://because remove warning (no sense....)
				break;
			}
		}
	}


	void PhysixCollider::UpdateTransform(const LibMath::Vector3& pos, const LibMath::Quaternion& rot)
	{
		if(m_type == RigidType::PLANE)
		{
			SetPosition(pos);
			return;
		}
		PxTransform transform(pos.x, pos.y, pos.z, { rot.X, rot.Y, rot.Z, rot.W });
		GetActor()->setGlobalPose(transform);
	}

	LibMath::Vector3 PhysixCollider::GetScaleShape()const
	{
		if(m_shape)
		{
			const auto type = m_shape->m_shape->getGeometry().getType();
			if (type == PxGeometryType::eBOX)
			{
				const auto boxExtends = m_shape->m_shape->getGeometry().box();
				return { boxExtends.halfExtents.x, boxExtends.halfExtents.y, boxExtends.halfExtents.z };
			}
			if (type == PxGeometryType::eCAPSULE)
			{
				const auto capsExtends = m_shape->m_shape->getGeometry().capsule();
				return { capsExtends.radius,capsExtends.halfHeight, (float)INVALID_INDEX };
			}
			if (type == PxGeometryType::eSPHERE)
			{
				const auto spheExtends = m_shape->m_shape->getGeometry().sphere();
				return { spheExtends.radius,(float)INVALID_INDEX, (float)INVALID_INDEX };
			}
		}
		return { (float)INVALID_INDEX,(float)INVALID_INDEX,(float)INVALID_INDEX };
		
	}
	
	LibMath::Vector3 PhysixCollider::GetPosition()
	{
		if (m_type == RigidType::PLANE)
		{
			return {0,0,0};
		}
		PxVec3 pos = GetActor()->getGlobalPose().p;
		return { pos.x, pos.y,pos.z };
	}

	LibMath::Quaternion PhysixCollider::GetRotator()
	{
		if (m_type == RigidType::PLANE)
		{
			return{0,0,0,1};
		}
		PxQuat rot =GetActor()->getGlobalPose().q;
		return LibMath::Quaternion{ rot.x,rot.y,rot.z,rot.w };
	}

	void PhysixCollider::SetPosition(const LibMath::Vector3& pos)
	{
	/*	if (m_type == RigidType::PLANE)
		{
			return;
		}*/
		PxTransform transform = GetActor()->getGlobalPose();
		transform.p = { pos.x,pos.y,pos.z };
		GetActor()->setGlobalPose(transform);
	}

	void PhysixCollider::SetRotator(const LibMath::Quaternion& rot)
	{
		if (m_type == RigidType::PLANE)
		{
			return;
		}
		PxTransform transform = GetActor()->getGlobalPose();
		transform.q = { rot.X,rot.Y,rot.Z, rot.W };
		GetActor()->setGlobalPose(transform);
	}
}
