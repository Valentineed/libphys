#pragma once
#include <cassert>
#include <cstdio>
#include <iostream>
#include <foundation/PxErrorCallback.h>

using namespace physx;
class ErrorCallback : public PxErrorCallback
{
public:
    virtual  void  reportError(PxErrorCode::Enum  code, const  char* message, const  char* file,
        int  line);
};
	
