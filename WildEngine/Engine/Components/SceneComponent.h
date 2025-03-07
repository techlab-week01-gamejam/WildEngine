#pragma once
#include "../../Core/Object/Object.h"
#include "../../Core/Math/Vector.h"
#include "../../Core/Math/Matrix.h"

class USceneComponent : public UObject
{
public:
	FVector RelativeLocation;
	FVector RelativeRotation;
	FVector RelativeScale3D;

public:

};