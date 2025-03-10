#include "GizmoComponent.h"
#include "Renderer/URenderer.h"

UGizmoComponent::UGizmoComponent(URenderer* InRenderer)
{
	Renderer = InRenderer;

    CurrentType = GizmoType::Translation;

    RelativeLocation.X = 1;
    RelativeLocation.Y = 1;
    RelativeLocation.Z = 1;
}

UGizmoComponent::~UGizmoComponent()
{

}

void UGizmoComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
    switch (CurrentType)
    {
        case GizmoType::Translation:
            
            NumVerticesX = sizeof(translationX_vertices) / sizeof(FVertexType);
            VertexBufferX = Renderer->CreateVertexBuffer(translationX_vertices, sizeof(translationX_vertices));

            NumVerticesY = sizeof(translationY_vertices) / sizeof(FVertexType);
            VertexBufferY = Renderer->CreateVertexBuffer(translationY_vertices, sizeof(translationY_vertices));

            NumVerticesZ = sizeof(translationZ_vertices) / sizeof(FVertexType);
            VertexBufferZ = Renderer->CreateVertexBuffer(translationZ_vertices, sizeof(translationZ_vertices));

            break;
        case GizmoType::Rotation:


            break;
        case GizmoType::Scale:

            break;
        default:

            break;
    }

    FMatrix localMatrix = FMatrix::Identity();

    FMatrix FinalMatrix = localMatrix * WorldMatrix;

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(FinalMatrix, ViewMatrix, ProjectionMatrix);

    Renderer->RenderGizmo(VertexBufferX, NumVerticesX);
    Renderer->RenderGizmo(VertexBufferY, NumVerticesY);
    Renderer->RenderGizmo(VertexBufferZ, NumVerticesZ);
}

void UGizmoComponent::SetGizmoType(GizmoType NewType)
{
    CurrentType = NewType;
}
