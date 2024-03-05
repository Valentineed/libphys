#pragma once
#include "libphys_Export.h"
#include "PxPhysicsAPI.h"

namespace LibMath {
	struct Vector3;
}

using namespace physx;
struct SceneQueryHit
{
	PxSceneQueryHit hit;
};
struct RaycastHit
{
	PxRaycastHit hit;
	PxSceneQueryFlags outputFlags;
};

struct SpherecastHit
{
	bool  blockingHit;
	static const  PxU32  bufferSize = 256;        
	PxRaycastHit  hitBuffer[bufferSize];
	
};
struct HitInfo
{
	
	[[nodiscard]] libphys_API float GetDistance()const;
	[[nodiscard]] libphys_API LibMath::Vector3 GetPosition()const;
	[[nodiscard]] libphys_API LibMath::Vector3 GetNormal() const;
	[[nodiscard]] libphys_API size_t GetEntityID() const;
	
	[[nodiscard]] PxRaycastBuffer& GetBuffer();
private:
	PxRaycastBuffer buffer;
};

struct Raycast
{
	PxVec3 origin;
	PxVec3 unitDir;
	PxReal maxDist;

	static libphys_API bool Ray(LibMath::Vector3 origin, LibMath::Vector3 dir, float maxDistance, HitInfo& hitInfo);
};

/*https://documentation.help/NVIDIA-PhysX-SDK-Guide/SceneQuery.html
 * // Raycast contre tous les objets statiques et dynamiques (pas de filtrage) 
// Le r�sultat principal de cet appel est le bool�en 'status' 
bool  status  =  scene -> raycastAny ( origin ,  unitDir ,  maxDistance ,  hit );
 */
/*
 * // Raycast contre tous les objets statiques et dynamiques (pas de filtrage) 
// Le r�sultat principal de cet appel est le hit le plus proche, stock� dans la structure 'hit' 
bool  status  =  scene -> raycastSingle ( origin ,  unitDir ,  maxDistance ,  outputFlags ,  hit );
 */
/*// Le r�sultat principal de cet appel sont tous les hits le long du rayon, stock�s dans 'hitBuffer'
PxI32  nbHits  =  scene -> raycastMultiple ( origin ,  unitDir ,  maxDistance ,  outputFlags ,
									  hitBuffer ,  bufferSize ,  blockingHit );*/

  /*/ [in] D�finissez les parties de PxRaycastHit qui nous int�ressent
 const  PxSceneQueryFlags  outputFlags  =  PxSceneQueryFlag :: eDISTANCE  |  PxSceneQueryFlag :: eIMPACT  |  PxSceneQueryFlag :: eNORMAL ;*/
