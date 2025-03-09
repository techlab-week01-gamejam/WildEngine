#include "CameraComponent.h"

UCameraComponent::UCameraComponent()
{
    FieldOfView = 90.0f;  // 기본 FOV
    NearZ = 0.1f;
    FarZ = 1000.0f;
}

FMatrix UCameraComponent::GetViewMatrix() const
{
    FMatrix RotationMatrix = FMatrix::CreateRotationRollPitchYaw(RelativeRotation.X, RelativeRotation.Y, RelativeRotation.Z);

    // 카메라 벡터 설정
    FVector Forward = (RotationMatrix * FVector(0.0f, 0.0f, 1.0f)).Normalized();
    FVector Up = RotationMatrix * FVector(0.0f, 1.0f, 0.0f);
    FVector Right = Up.Cross(Forward).Normalized();
    FVector NewUp = Forward.Cross(Right);

    FMatrix View;

    View.M[0][0] = Right.X;  View.M[0][1] = Right.Y;  View.M[0][2] = Right.Z;  View.M[0][3] = -Right.Dot(RelativeLocation);
    View.M[1][0] = NewUp.X;  View.M[1][1] = NewUp.Y;  View.M[1][2] = NewUp.Z;  View.M[1][3] = -NewUp.Dot(RelativeLocation);
    View.M[2][0] = Forward.X; View.M[2][1] = Forward.Y; View.M[2][2] = Forward.Z; View.M[2][3] = -Forward.Dot(RelativeLocation);
    View.M[3][3] = 1.0f;

    return View;
}

FMatrix UCameraComponent::GetProjectionMatrix(float AspectRatio) const
{
    FMatrix ProjectionMatrix;

    float FOVRadians = FieldOfView * (3.141592f / 180.0f); // FOV를 라디안 값으로 변환
    float Height = tan(FOVRadians / 2.0f);
    float Width = Height / AspectRatio;
    float zRange = FarZ / (FarZ - NearZ);

    ProjectionMatrix.M[0][0] = Width;
    ProjectionMatrix.M[1][1] = Height;
    ProjectionMatrix.M[2][2] = zRange;
    ProjectionMatrix.M[2][3] = 1.0f;
    ProjectionMatrix.M[3][2] = -zRange * NearZ;

    return ProjectionMatrix;
}


void UCameraComponent::SetFieldOfView(float NewFOV)
{
    FieldOfView = NewFOV;
}

void UCameraComponent::SetNearFarPlane(float NewNearZ, float NewFarZ)
{
    NearZ = NewNearZ;
    FarZ = NewFarZ;
}

float UCameraComponent::GetFieldOfView() const
{
    return FieldOfView;
}

float UCameraComponent::GetNearZ() const
{
    return NearZ;
}

float UCameraComponent::GetFarZ() const
{
    return FarZ;
}