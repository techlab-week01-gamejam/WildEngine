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
}

void UTriangleComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
    if (rot == 180) rot = 0;
    rot += 0.01f;

    FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
    FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(rot, 0, rot);
    FMatrix Scaling = FMatrix::Scaling(1, 1, 1);

    FMatrix World = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(World, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);
}

