#pragma once
#include "../../Core/Object/Object.h"
#include "../../Core/Math/FVector.h"

class USceneComponent : public UObject
{
public:
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;
};