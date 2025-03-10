#pragma once
#include "Types/Types.h"
#include "Class/Class.h"
#include "ObjectManager.h"

class UObject
{
public:
	UObject();
	virtual ~UObject();

	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray

	static UClass* GetClass(); // 클래스에서 호출 가능

	// new 연산자 오버로딩
	void* operator new(size_t size);
	void operator delete(void* ptr);
};