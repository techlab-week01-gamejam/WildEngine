#pragma once
#include "Math/Vector.h"
#include "Components/SceneComponent.h"

struct FVertexType
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

enum class EPrimitiveType
{
    Cube,
    Sphere,
    Triangle
};

enum class EGizmoType
{
    Translation,
    Rotation,
    Scale
};

struct FHitResult
{
    bool bHit;         // �浹 ����
    float Distance;    // �浹�� ��ġ������ �Ÿ�
    FVector HitLocation; // �浹 ��ġ
    USceneComponent* HitObject; // �浹�� ������Ʈ

    FHitResult() : bHit(false), Distance(FLT_MAX), HitLocation(), HitObject(nullptr) {}
};