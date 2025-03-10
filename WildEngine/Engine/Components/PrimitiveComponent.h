#pragma once
#include "SceneComponent.h"
#include "Primitive.h"

class UPrimitiveComponent : public USceneComponent
{
public:
	UPrimitiveComponent();
	~UPrimitiveComponent();

	virtual bool CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult) = 0;

	static UClass* GetClass();
};