#include "SceneComponent.h"

USceneComponent::USceneComponent()
{
    RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
    RelativeRotation = FVector(0.0f, 0.0f, 0.0f);
    RelativeScale3D = FVector(1.0f, 1.0f, 1.0f);
}


void USceneComponent::SetRelativeLocation(const FVector& NewLocation)
{
    RelativeLocation = NewLocation;
}

void USceneComponent::SetRelativeRotation(const FVector& NewRotation)
{
    RelativeRotation = NewRotation;
}

void USceneComponent::SetRelativeScale3D(const FVector& NewScale)
{
    RelativeScale3D = NewScale;
}

FVector USceneComponent::GetRelativeLocation() const
{
    return RelativeLocation;
}

FVector USceneComponent::GetRelativeRotation() const
{
    return RelativeRotation;
}

FVector USceneComponent::GetRelativeScale3D() const
{
    return RelativeScale3D;
}