#pragma once
#include "PrimitiveComponent.h"

#include "Types/Types.h"
#include "Math/Matrix.h"

class URenderer;

class UTriangleComponent : public UPrimitiveComponent
{
public:
	UTriangleComponent(URenderer* InRenderer);
	UTriangleComponent(const UTriangleComponent&);
	~UTriangleComponent();

	void Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix);

	bool CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult) override;

	// �ﰢ�� ������ ���� �� ��������


private:
	void Initialize();

private:
	URenderer* Renderer;

	UINT NumVertices;
	ID3D11Buffer* VertexBuffer;

	float rot;
	FVector V0, V1, V2;
};

