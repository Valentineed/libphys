#pragma once
#include <PxForceMode.h>

enum SelectMode
{
	FORCE,				//!< parameter has unit of mass * distance/ time^2, i.e. a force
	IMPULSE,			//!< parameter has unit of mass * distance /time
	VELOCITY_CHANGE,	//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
	ACCELERATION		//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
};
struct ForceMode
{
	ForceMode(SelectMode mode)
	{
		switch (mode)
		{
		case FORCE:
			myForce = fmode.eFORCE;
			break;
		case IMPULSE:
			myForce = fmode.eIMPULSE;
			break;
		case VELOCITY_CHANGE:
			myForce = fmode.eVELOCITY_CHANGE;
			break;
		case ACCELERATION:
			myForce = fmode.eACCELERATION;
			break;
			default:
				myForce = fmode.eFORCE;
		}
	}

	physx::PxForceMode::Enum myForce;
private:
	physx::PxForceMode fmode;
};