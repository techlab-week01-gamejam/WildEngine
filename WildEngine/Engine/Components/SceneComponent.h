#pragma once

#include "Object/Object.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

class URenderer;

class USceneComponent : public UObject
{
public:
	USceneComponent();
	USceneComponent(URenderer* InRenderer, const FVector& InLocation){};

	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D = FVector(1, 1, 1);

	FMatrix WorldTransform;

	static UClass* GetClass();
	UClass* GetInstanceClass() const override;

public:
	virtual FMatrix GetWorldTransform() { return WorldTransform; };
};