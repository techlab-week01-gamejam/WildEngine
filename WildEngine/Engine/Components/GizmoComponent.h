#pragma once
#include "Components/SceneComponent.h"
#include "Types/CommonTypes.h"

class URenderer;


class UGizmoComponent : public USceneComponent {
public:
	UGizmoComponent();
	UGizmoComponent(URenderer* InRenderer);

	void Render(USceneComponent* Obj, FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix);

private:
	USceneComponent* SelectedObj = nullptr;
	EGizmoType GizmoType = EGizmoType::Translation;

	URenderer* Renderer;

	uint32 NumVertices;
	uint32 NumIndices;
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* GizmoIndexBuffer;

};