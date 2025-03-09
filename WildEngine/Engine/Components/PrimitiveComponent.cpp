#include "PrimitiveComponent.h"

UPrimitiveComponent::UPrimitiveComponent()
{
}

FMatrix UPrimitiveComponent::GetModelMatrix() const
{
    FMatrix Scale, Rotation, Translation, Model;

    Scale = FMatrix::Scaling(RelativeScale3D.X, RelativeScale3D.Y, RelativeScale3D.Z);
    Rotation = FMatrix::CreateRotationRollPitchYaw(RelativeRotation.X, RelativeRotation.Y, RelativeRotation.Z);
    Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);

    Model = Scale * Rotation * Translation;
    return Model;
}

FMatrix UPrimitiveComponent::GetMVPMatrix() const
{
    return MVPMatrix;
}

void UPrimitiveComponent::SetMVPMatrix(const FMatrix& MVP)
{
    MVPMatrix = MVP;
}
