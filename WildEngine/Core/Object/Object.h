#pragma once
#include "../Types/Types.h"

class UObject
{
public:
	virtual ~UObject() {}	//RTTI ȣ���� ���� ���� �Ҹ��� �߰�
	uint32 UUID;
	uint32 InternalIndex; // Index of GUObjectArray
};