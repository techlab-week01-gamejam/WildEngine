#pragma once
#include "Components/SceneComponent.h"

class UCameraComponent : public USceneComponent {

public:
    UCameraComponent();

    FMatrix GetViewMatrix() const;       // 카메라의 View Matrix 반환
    FMatrix GetProjectionMatrix(float AspectRatio) const; // 카메라의 Projection Matrix 반환

    // 카메라 설정 함수
    void SetFieldOfView(float NewFOV);
    void SetNearFarPlane(float NewNearZ, float NewFarZ);

    float GetFieldOfView() const;
    float GetNearZ() const;
    float GetFarZ() const;

    //bool bUsePerspective; // 원근 투영(Perspective) 여부(Perspective / Orthogonal)

private:
    float FieldOfView;    // 카메라의 FOV (시야각)
    float NearZ;  // Near Clipping Plane 거리
    float FarZ;   // Far Clipping Plane 거리
};