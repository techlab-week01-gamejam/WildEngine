#include "Components/CubeComponent.h"

#include "Renderer/URenderer.h"

UCubeComponent::UCubeComponent()
{
}

UCubeComponent::UCubeComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}

UCubeComponent::UCubeComponent(const UCubeComponent&)
{
}

void UCubeComponent::Initialize()
{
    NumVertices = sizeof(cube_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));

    RelativeLocation.X = 0.0f;
    RelativeLocation.Y = 0.0f;
    RelativeLocation.Z = 3.0f;

    CubeSize = 1.0f;
}

UCubeComponent::~UCubeComponent()
{
}

//UClass* UCubeComponent::GetClass()
//{
//    static UClass CubeClass("UCubeComponent", []() -> UObject* { return new UCubeComponent(); });
//    return &CubeClass;
//}

void UCubeComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
    FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
    FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(0, rot, 0);
    FMatrix Scaling = FMatrix::Scaling(CubeSize, CubeSize, CubeSize);

    WorldTransform = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(WorldTransform, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);
}

bool UCubeComponent::CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult)
{
    //// 1) 먼저 월드 변환 행렬을 얻고, 역행렬을 구합니다.
    ////    GetWorldTransform()는 스케일, 회전, 위치가 모두 적용된 월드 행렬입니다.
    //const FMatrix WorldMatrix = GetWorldTransform();
    //FMatrix InverseWorldMatrix = WorldMatrix.Inverse();

    //// 2) Ray를 큐브의 로컬 공간으로 변환합니다.
    ////    (위치는 좌표 변환, 방향은 벡터 변환)
    //FVector LocalRayOrigin = InverseWorldMatrix * RayOrigin;
    //FVector LocalRayDirection = (InverseWorldMatrix.TransformDirection(InverseWorldMatrix, RayDirection)).Normalized();

    //// 3) 로컬 공간에서 AABB 충돌 검사
    ////    큐브는 로컬 공간에서 -HalfSize ~ +HalfSize 범위를 가정합니다.
    //FVector HalfSize = (RelativeScale3D * 0.5f);
    //FVector BoxMin = -HalfSize;
    //FVector BoxMax = HalfSize;

    //float tMin = (BoxMin.X - LocalRayOrigin.X) / LocalRayDirection.X;
    //float tMax = (BoxMax.X - LocalRayOrigin.X) / LocalRayDirection.X;
    //if (tMin > tMax) std::swap(tMin, tMax);

    //float tyMin = (BoxMin.Y - LocalRayOrigin.Y) / LocalRayDirection.Y;
    //float tyMax = (BoxMax.Y - LocalRayOrigin.Y) / LocalRayDirection.Y;
    //if (tyMin > tyMax) std::swap(tyMin, tyMax);

    //if ((tMin > tyMax) || (tyMin > tMax))
    //    return false;

    //if (tyMin > tMin) tMin = tyMin;
    //if (tyMax < tMax) tMax = tyMax;

    //float tzMin = (BoxMin.Z - LocalRayOrigin.Z) / LocalRayDirection.Z;
    //float tzMax = (BoxMax.Z - LocalRayOrigin.Z) / LocalRayDirection.Z;
    //if (tzMin > tzMax) std::swap(tzMin, tzMax);

    //if ((tMin > tzMax) || (tzMin > tMax))
    //    return false;

    //// 교차 지점이 존재한다면, tMin이 충돌 거리
    //if (tzMin > tMin)
    //    tMin = tzMin;
    //if (tzMax < tMax)
    //    tMax = tzMax;

    //// 4) 충돌 결과를 월드 공간 기준으로 다시 변환
    //OutHitResult.bHit = true;
    //OutHitResult.Distance = tMin;
    //// 로컬 충돌 위치를 월드 공간으로 변환
    //FVector LocalHitPos = LocalRayOrigin + (LocalRayDirection * tMin);
    //FVector WorldHitPos = WorldMatrix * LocalHitPos;
    //OutHitResult.HitLocation = WorldHitPos;
    //OutHitResult.HitObject = this;

    //return true;

    return false;
}


