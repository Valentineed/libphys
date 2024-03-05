#pragma once
namespace physx {
	class PxRevoluteJoint;
	class PxTransform;
	class PxRigidActor;
}

using namespace physx;
class Joint
{
public:
	/*
	 * a fixed joint locks the orientations and origins rigidly together
	 * a distance joint keeps the origins within a certain distance range
	 * a spherical joint (also called a ball-and-socket) keeps the origins together,
	 but allows the orientations to vary freely.
	 * a revolute joint (also called a hinge) keeps the origins and x-axes of the frames
	 together, and allows free rotation around this common axis.
	 * a prismatic joint (also called a slider) keeps the orientations identical,
	 but allows the origin of each frame to slide freely along the common x-axis.
	 * a D6 joint is a highly configurable joint that allows specification of individual
	 degrees of freedom either to move freely or be locked together.
	 It can be used to implement a wide variety of mechanical and anatomical joints,
	 but is somewhat less intuitive to configure than the other joint types.
	 This joint is covered in detail below.
	 */

	//comme dans le reste de l'API PhysX, tous les angles d'articulation des
	//limites et des cibles d'entraînement sont spécifiés en radians
	
	void CreateJoint(PxRigidActor* actor0, const PxTransform& tranform0, PxRigidActor* actor1, const PxTransform& tranform1);

	void SetBreakForce(float force, float torque) const;
	
	void SetVisualization(bool isVisible) const;

	bool IsBroken() const;

	void SetProjectionLinearTolerance(float tolerance) const;

	void SetProjectionAngularTolerance(float tolerance) const;

	void ActiveProjection(bool isActive) const;

	void SetLimit(float upper, float lower, float tolerance) const;
	void SetDefaultLimit() const;
	
	void ActiveLimit(bool isActive)const;
	
	PxRevoluteJoint* m_joint = nullptr;
};
