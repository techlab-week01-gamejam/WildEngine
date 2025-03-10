#pragma once
#include "PrimitiveComponent.h"

#include "Types/Types.h"
#include "Math/Matrix.h"

class URenderer;

class UCubeComponent : public UPrimitiveComponent
{

public:
	UCubeComponent();
	UCubeComponent(URenderer* InRenderer);
	UCubeComponent(URenderer* InRenderer, FVector Location);
	UCubeComponent(URenderer* InRenderer, FVector Location, FVector Rotation);
	UCubeComponent(URenderer* InRenderer, FVector Location, FVector Rotation, FVector Scale);
	UCubeComponent(const UCubeComponent&);
	~UCubeComponent();

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