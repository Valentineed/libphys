#pragma once
#include "PTransform.h"
#include "Actors/RigidType.h"
#include "libphys_Export.h"
#include <functional>
#include <sstream> 

#include "Defines.h"
#include "PxPhysicsAPI.h"
#include "PhysicsMaterial/Material.h"

struct ForceMode;
class TriangleMesh;
struct TriangleMeshDescriptor;

namespace physx {
	class PxRigidActor;
}

struct StaticActor;
struct DynamicActor;
class Shape;

template <typename>
class  ColliderDelegate;

template <typename rType, typename... Args>
class ColliderDelegate<rType(Args ...)>
{
public:
	libphys_API ColliderDelegate() = default;
	template<typename pClass>
	void Connect(pClass* t, rType(pClass::* method)(Args...))
	{
		m_function = [=](Args ... as) { (t->*method)(std::forward<Args>(as)...); };
	}

	rType operator() (Args... args)
	{
		return m_function(std::forward<Args>(args)...);
	}

protected:
	std::function<rType(Args ...)> m_function = {};
};

namespace Physix
{
	class PhysixCollider
	{
	public:
		libphys_API PhysixCollider() = default;
		libphys_API PhysixCollider(Shape* shape, RigidType type, const PTransform& transform, size_t entityID);
		libphys_API PhysixCollider(TriangleMesh& triangleMesh, RigidType type, const PTransform& transform, size_t entityID);
		libphys_API PhysixCollider(float nx, float ny, float nz, float dist, const PTransform& transform, size_t entityID);
		libphys_API PhysixCollider(PhysixCollider&& other) noexcept;
		libphys_API PhysixCollider& operator=(PhysixCollider&& other);
		~PhysixCollider();

		bool libphys_API operator==(const PhysixCollider& rhs) const;

		
		void libphys_API ChangeType(RigidType type);
		void libphys_API SetKinematic(bool isKinematic) const;
		void libphys_API Release();
		[[nodiscard]] LibMath::Vector3 libphys_API GetLinearVelocity() const;
		[[nodiscard]] LibMath::Vector3 libphys_API GetAngularVelocity() const;
		[[nodiscard]] float libphys_API GetMass() const;
		void libphys_API SetScale(LibMath::Vector3& scale);
		void libphys_API SetLinearVelocity(const LibMath::Vector3& linVel) const;
		void libphys_API SetAngularVelocity(const LibMath::Vector3& angVel) const;
		void libphys_API DisableGravity(bool disable) const;
		void libphys_API SetMass(float mass) const;
		void libphys_API SetMassSpaceInertiaTensor(const LibMath::Vector3& mass) const;
		void libphys_API AddForce(const LibMath::Vector3& force, const ForceMode& mode) const;
		void libphys_API AddTorque(const LibMath::Vector3& force, const ForceMode& mode) const;
		void libphys_API SetAngularDamping(float angDamp) const;
		void libphys_API SetLinearDamping(float linDamp) const;
		void libphys_API ClearTorque(const ForceMode& mode) const;
		void libphys_API ClearForce(const ForceMode& mode) const;

		void libphys_API AddForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;
		void libphys_API AddForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;
		void libphys_API AddLocalForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;
		void libphys_API AddLocalForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;

		void libphys_API OnContact(size_t indexOtherActor);

		void libphys_API EndContact(size_t indexOtherActor);

		void libphys_API SetName(const char* name) const;
		const char* GetName() const;
		
		physx::PxRigidActor* GetActor() const;

		template<typename pClass>
		void BindOnBeginContact(pClass* pclass, void(pClass::* method)(size_t));

		template<typename pClass>
		void BindOnEndContact(pClass* pclass, void(pClass::* method)(size_t));

		void libphys_API PutToSleep();

		void libphys_API LockRotation(bool lockX, bool lockY, bool lockZ);
		void libphys_API LockLocation(bool lockX, bool lockY, bool lockZ);
		//Collision Settings
		void libphys_API ShapeInShapePairIntersectionTests(bool active);
		void libphys_API ShapeInSceneQueryTests(bool active);
		void libphys_API TriggerCollision(bool bisTrigger) const;

		
		LibMath::Vector3 libphys_API GetPosition();
		LibMath::Quaternion libphys_API GetRotator();

		void libphys_API SetPosition(const LibMath::Vector3& pos);
		void libphys_API SetRotator(const LibMath::Quaternion& rot);
		
		void libphys_API UpdateTransform(const LibMath::Vector3& pos, const LibMath::Quaternion& rot);
		LibMath::Vector3 libphys_API GetScaleShape()const;


		ColliderDelegate<void(size_t)> m_onBeginContactDelegate;
		ColliderDelegate<void(size_t)> m_onEndContactDelegate;

		RigidType m_type = RigidType::STATIC;
		size_t m_idEntity = INVALID_INDEX;
	private:
		Shape* m_shape = nullptr;
		PTransform m_transform;

		DynamicActor* m_dynamic = nullptr;
		StaticActor* m_static = nullptr;


		Material m_material;
		bool bSetBeginContact = false;
		bool bSetEndContact = false;
	};

	template <typename pClass>
	void PhysixCollider::BindOnBeginContact(pClass* pclass, void( pClass::* method)(size_t))
	{
		m_onBeginContactDelegate.Connect(pclass, method);
		bSetBeginContact = true;
	}

	template <typename pClass>
	void PhysixCollider::BindOnEndContact(pClass* pclass, void( pClass::* method)(size_t))
	{
		m_onEndContactDelegate.Connect(pclass, method);
		bSetEndContact = true;
	}
}
