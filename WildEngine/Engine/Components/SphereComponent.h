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
	~USphereComponent();

	static UClass* GetClass();

	void Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix);

	bool CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult) override;

	void SetRadius(float InRadius)
	{
		Radius = InRadius;
	}
	
	float GetRadius() const
	{
		return Radius;
	}

private:
	void Initialize();

private:
	URenderer* Renderer;

	UINT NumVertices;
	ID3D11Buffer* VertexBuffer;

	float Radius = 1.0f;
	float rot;
};