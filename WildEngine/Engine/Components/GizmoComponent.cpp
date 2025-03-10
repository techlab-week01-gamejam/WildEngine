#include "GizmoComponent.h"
#include "Renderer/URenderer.h"
#include "Gizmo.h"

UGizmoComponent::UGizmoComponent() 
{
}

UGizmoComponent::UGizmoComponent(URenderer* InRenderer)
{
	Renderer = InRenderer;
	InitializeGizmoArrow(1.0f, 0.05f, 0.2f, 0.08f, 1.0f, 0.0f, 0.0f);  // X축 (빨강)
	InitializeGizmoArrow(1.0f, 0.05f, 0.2f, 0.08f, 0.0f, 1.0f, 0.0f);  // X축 (빨강)
	InitializeGizmoArrow(1.0f, 0.05f, 0.2f, 0.08f, 0.0f, 0.0f, 1.0f);  // X축 (빨강)
	NumVertices = gizmo_translation_vertices.size();
	NumIndices = gizmo_translation_indices.size();
	VertexBuffer = Renderer->CreateVertexBuffer(gizmo_translation_vertices.data(), gizmo_translation_vertices.size() * sizeof(FVertexType));
	GizmoIndexBuffer = Renderer->CreateIndexBuffer(gizmo_translation_indices);
}

void UGizmoComponent::Render(USceneComponent* Obj, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
	RelativeLocation = Obj->RelativeLocation;
	RelativeRotation = Obj->RelativeRotation;
	RelativeScale3D = Obj->RelativeScale3D;

	FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
	FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(RelativeRotation.X, RelativeRotation.Y, RelativeRotation.Z);
	FMatrix Scaling = FMatrix::Scaling(1, 1, 1);

	FMatrix World = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

	// 셰이더 상수 버퍼 업데이트
	Renderer->UpdateShaderParameters(World, ViewMatrix, ProjectionMatrix);

	Renderer->RenderGizmo(VertexBuffer, GizmoIndexBuffer, NumVertices, NumIndices);
}
