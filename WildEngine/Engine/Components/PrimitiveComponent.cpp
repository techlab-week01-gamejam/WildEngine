#include "PrimitiveComponent.h"

UPrimitiveComponent::UPrimitiveComponent()
{
}

UPrimitiveComponent::~UPrimitiveComponent()
{
}

UClass* UPrimitiveComponent::GetClass()
{
	static UClass PrimitiveClass("UPrimitiveComponent", []() -> UObject* { return new UPrimitiveComponent(); });
	return &PrimitiveClass;
}
