#pragma once
#include "Math/Vector.h"
#include "Object/Object.h"

struct FVertexType
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

enum class PrimitiveType
{
    Cube,
    Sphere,
    Triangle
};

struct FHitResult
{
    bool bHit;         // �浹 ����
    float Distance;    // �浹�� ��ġ������ �Ÿ�
    FVector HitLocation; // �浹 ��ġ
    UObject* HitObject; // �浹�� ������Ʈ

    FHitResult() : bHit(false), Distance(FLT_MAX), HitLocation(), HitObject(nullptr) {}
};