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
	UTriangleComponent(URenderer* InRenderer, FVector Location);
	UTriangleComponent(URenderer* InRenderer, FVector Location, FVector Rotation);
	UTriangleComponent(URenderer* InRenderer, FVector Location, FVector Rotation, FVector Scale);


	~UTriangleComponent();

	void Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix);

private:
	void Initialize();

private:
	URenderer* Renderer;

	UINT NumVertices;
	ID3D11Buffer* VertexBuffer;

	float rot;
};

