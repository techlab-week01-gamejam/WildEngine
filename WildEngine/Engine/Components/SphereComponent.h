#pragma once
#include "PrimitiveComponent.h"

#include "Types/Types.h"
#include "Math/Matrix.h"

class URenderer;

class USphereComponent : public UPrimitiveComponent
{

public:
	USphereComponent();
	USphereComponent(URenderer* InRenderer);
	USphereComponent(const USphereComponent&);

	USphereComponent(URenderer* InRenderer, FVector Location);
	USphereComponent(URenderer* InRenderer, FVector Location, FVector Rotation);
	USphereComponent(URenderer* InRenderer, FVector Location, FVector Rotation, FVector Scale);


	~USphereComponent();

	static UClass* GetClass();

	void Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix);

private:
	void Initialize();

private:
	URenderer* Renderer;

	UINT NumVertices;
	ID3D11Buffer* VertexBuffer;

	float rot;
};