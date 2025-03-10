#pragma once

#include "Gizmo.h"
#include "SceneComponent.h"

class URenderer;

enum class GizmoType
{
	Translation,
	Rotation,
	Scale
};

class UGizmoComponent : public USceneComponent
{
public:
	UGizmoComponent(URenderer* InRenderer);
	UGizmoComponent(const UGizmoComponent&) = delete;
	~UGizmoComponent();

	void Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix);

	void SetGizmoType(GizmoType NewType);
private:
	URenderer* Renderer;

	GizmoType CurrentType;
	
	UINT NumVerticesX;
	ID3D11Buffer* VertexBufferX;

	UINT NumVerticesY;
	ID3D11Buffer* VertexBufferY;

	UINT NumVerticesZ;
	ID3D11Buffer* VertexBufferZ;
};

