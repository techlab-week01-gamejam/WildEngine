#include "SphereComponent.h"

#include "Renderer/URenderer.h"

USphereComponent::USphereComponent()
{
}

USphereComponent::USphereComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}

USphereComponent::USphereComponent(const USphereComponent&)
{
}


void USphereComponent::Initialize()
{
    NumVertices = sizeof(sphere_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));

    RelativeLocation.X = 0.0f;
    RelativeLocation.Y = 0.0f;
    RelativeLocation.Z = 3.0f;

    Radius = 1.0f;
}

USphereComponent::~USphereComponent()
{
}

//UClass* USphereComponent::GetClass()
//{
//    static UClass SphereClass("USphereComponent", []() -> UObject* { return new USphereComponent(); });
//    return &SphereClass;
//}

void USphereComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
    FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
    FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(rot, 0, rot);
    FMatrix Scaling = FMatrix::Scaling(Radius, Radius, Radius);

    FMatrix World = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(World, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);

}

bool USphereComponent::CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult)
{
    FVector SphereCenter = RelativeLocation; // 구의 월드 좌표
    float SphereRadius = Radius; // 구의 반지름

    FVector L = SphereCenter - RayOrigin;
    float tca = L.Dot(RayDirection);
    float d2 = L.Dot(L) - tca * tca;
    float radius2 = SphereRadius * SphereRadius;

    if (d2 > radius2)
        return false; // 충돌 없음

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0 && t1 < 0)
        return false;

    OutHitResult.bHit = true;
    OutHitResult.Distance = (t0 < 0) ? t1 : t0;
    OutHitResult.HitLocation = RayOrigin + RayDirection * OutHitResult.Distance;
    OutHitResult.HitObject = this;

    return true;
}


