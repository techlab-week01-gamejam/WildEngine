#pragma once
#include "../Types/Types.h"

class UObject
{
public:
	virtual ~UObject() {}	//RTTI 호출을 위한 가상 소멸자 추가
	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray
};