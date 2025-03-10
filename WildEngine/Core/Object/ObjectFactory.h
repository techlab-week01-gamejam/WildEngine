#pragma once
#include "Object/Object.h"
#include "Object/ObjectManager.h"
#include "Interface/ISingleton.h"

class UObjectFactory : public ISingleton<UObjectFactory>{
public:
	static UObjectFactory& GetInsantce();

	UObject* ConstructObject(UClass* ClassType);

private:
	friend class ISingleton<UObjectFactory>;
	UObjectFactory() = default;

};