#pragma once
#include "Components/SceneComponent.h"

class UCameraComponent : public USceneComponent {

public:
    UCameraComponent();

    FMatrix GetViewMatrix() const;       // ī�޶��� View Matrix ��ȯ
    FMatrix GetProjectionMatrix(float AspectRatio) const; // ī�޶��� Projection Matrix ��ȯ

    // ī�޶� ���� �Լ�
    void SetFieldOfView(float NewFOV);
    void SetNearFarPlane(float NewNearZ, float NewFarZ);

    float GetFieldOfView() const;
    float GetNearZ() const;
    float GetFarZ() const;

    //bool bUsePerspective; // ���� ����(Perspective) ����(Perspective / Orthogonal)

private:
    float FieldOfView;    // ī�޶��� FOV (�þ߰�)
    float NearZ;  // Near Clipping Plane �Ÿ�
    float FarZ;   // Far Clipping Plane �Ÿ�
};