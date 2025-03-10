#include "TriangleComponent.h"

#include "Renderer/URenderer.h"

UTriangleComponent::UTriangleComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}

UTriangleComponent::UTriangleComponent(const UTriangleComponent&)
{
}

UTriangleComponent::~UTriangleComponent()
{
}

void UTriangleComponent::Initialize()
{
    NumVertices = sizeof(triangle_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));

    RelativeLocation.X = 0.0f;
    RelativeLocation.Y = 0.0f;
    RelativeLocation.Z = 3.0f;

    RelativeScale3D.X = 1.0f;
	RelativeScale3D.Y = 1.0f;
    RelativeScale3D.Z = 1.0f;

	V0 = FVector(0.0f, 1.0f, 0.0f);
	V1 = FVector(1.0f, -1.0f, 0.0f);
	V2 = FVector(-1.0f, -1.0f, 0.0f);
}

void UTriangleComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
    FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
    FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(rot, 0, rot);
    FMatrix Scaling = FMatrix::Scaling(RelativeScale3D.X, RelativeScale3D.Y, RelativeScale3D.Y);

    FMatrix World = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(World, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);
}

bool UTriangleComponent::CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult)
{
    return false;
}

