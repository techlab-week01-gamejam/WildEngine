#pragma once
#include "Object/Object.h"
#include "Object/ObjectManager.h"

class UObjectFactory {
public:
	static UObjectFactory& GetInsantce();

	UObject* ConstructObject(UClass* ClassType);

private:
	UObjectFactory() = default;

};