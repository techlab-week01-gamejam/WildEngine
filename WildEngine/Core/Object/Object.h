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

	static UClass* GetClass(); // Ŭ�������� ȣ�� ����

	// new ������ �����ε�
	void* operator new(size_t size);
	void operator delete(void* ptr);
};