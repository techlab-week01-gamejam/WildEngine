#include "PrimitiveComponent.h"

UPrimitiveComponent::UPrimitiveComponent()
{
}

UPrimitiveComponent::~UPrimitiveComponent()
{
}

UClass* UPrimitiveComponent::GetClass()
{
	static UClass PrimitiveClass("UPrimitiveComponent", UObject::GetClass());
	return &PrimitiveClass;
}

UClass* UPrimitiveComponent::GetInstanceClass() const
{
	return GetClass();
}

