#pragma once

#include "Object/Object.h"
#include "Math/Matrix.h"

class USceneComponent : public UObject
{
public:
	USceneComponent();

	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D = FVector(1, 1, 1);

	FMatrix WorldTransform;

	static UClass* GetClass();
	UClass* GetInstanceClass() const override;

public:
	virtual FMatrix GetWorldTransform() { return WorldTransform; };
};