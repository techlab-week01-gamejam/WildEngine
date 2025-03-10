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

    WorldTransform = Scaling * Rotation * Translation; // �����ϸ� * ȸ�� * �̵�

    // ���̴� ��� ���� ������Ʈ
    Renderer->UpdateShaderParameters(WorldTransform, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);
}

bool UCubeComponent::CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult)
{
    //// 1) ���� ���� ��ȯ ����� ���, ������� ���մϴ�.
    ////    GetWorldTransform()�� ������, ȸ��, ��ġ�� ��� ����� ���� ����Դϴ�.
    //const FMatrix WorldMatrix = GetWorldTransform();
    //FMatrix InverseWorldMatrix = WorldMatrix.Inverse();

    //// 2) Ray�� ť���� ���� �������� ��ȯ�մϴ�.
    ////    (��ġ�� ��ǥ ��ȯ, ������ ���� ��ȯ)
    //FVector LocalRayOrigin = InverseWorldMatrix * RayOrigin;
    //FVector LocalRayDirection = (InverseWorldMatrix.TransformDirection(InverseWorldMatrix, RayDirection)).Normalized();

    //// 3) ���� �������� AABB �浹 �˻�
    ////    ť��� ���� �������� -HalfSize ~ +HalfSize ������ �����մϴ�.
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

    //// ���� ������ �����Ѵٸ�, tMin�� �浹 �Ÿ�
    //if (tzMin > tMin)
    //    tMin = tzMin;
    //if (tzMax < tMax)
    //    tMax = tzMax;

    //// 4) �浹 ����� ���� ���� �������� �ٽ� ��ȯ
    //OutHitResult.bHit = true;
    //OutHitResult.Distance = tMin;
    //// ���� �浹 ��ġ�� ���� �������� ��ȯ
    //FVector LocalHitPos = LocalRayOrigin + (LocalRayDirection * tMin);
    //FVector WorldHitPos = WorldMatrix * LocalHitPos;
    //OutHitResult.HitLocation = WorldHitPos;
    //OutHitResult.HitObject = this;

    //return true;

    return false;
}


