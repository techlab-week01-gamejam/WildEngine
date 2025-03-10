#pragma once

#include "Object/Object.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

class USceneComponent : public UObject
{
public:
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;

	FMatrix WorldTransform;

public:
	virtual FMatrix GetWorldTransform() { return WorldTransform; };

};