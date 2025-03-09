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

public:
	USceneComponent();

    // ��ġ, ȸ��, ũ�� ����
    void SetRelativeLocation(const FVector& NewLocation);
    void SetRelativeRotation(const FVector& NewRotation);
    void SetRelativeScale3D(const FVector& NewScale);

    FVector GetRelativeLocation() const;
    FVector GetRelativeRotation() const;
    FVector GetRelativeScale3D() const;

};