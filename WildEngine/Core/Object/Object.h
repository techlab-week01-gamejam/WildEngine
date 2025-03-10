#pragma once
#include "Types/Types.h"
#include "Class/Class.h"

class UObject
{
public:
	UObject();
	virtual ~UObject();

	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray

	// ���� Ŭ���� ���� ��ȯ
	static UClass* GetClass();

	// ��ü�� ���� Ŭ���� ���� ��ȯ
	virtual UClass* GetInstanceClass() const { return GetClass(); }

	// Ư�� Ŭ�������� Ȯ��
	bool IsA(UClass* ClassType) const {
		return GetInstanceClass()->IsChildOf(ClassType);
	}

	// new ������ �����ε�
	void* operator new(size_t size);
	void operator delete(void* ptr);
};