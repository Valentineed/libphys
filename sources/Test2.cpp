#include "Test2.h"
#include <cstdio>
#include "extensions/PxDefaultErrorCallback.h"
#include "PxPhysicsAPI.h"

using namespace physx;

class AllocatorCallback
{
public:
	AllocatorCallback() = default;
    virtual ~AllocatorCallback() {}
    virtual void* allocate(size_t size, const char* typeName, const char* filename,
        int line) = 0;
    virtual void deallocate(void* ptr) = 0;
};

class DefaultAllocator : public PxAllocatorCallback 
{
public:
	void* allocate(size_t size, const char*, const char*, int)
	{
		void* ptr = platformAlignedAlloc(size);
		PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
		return ptr;
	}

	void deallocate(void* ptr)
	{
		platformAlignedFree(ptr);
	}
	
};
void TestLibphys::TestMessageLibphys()
{
}