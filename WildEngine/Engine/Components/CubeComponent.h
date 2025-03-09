#pragma once
#include "PrimitiveComponent.h"
#include "Renderer/URenderer.h"
#include "Types/Types.h"

class UCubeComponent : public UPrimitiveComponent
{

public:
	UCubeComponent(URenderer* InRenderer);

	virtual void Render() override;
	void InitializeMesh();

private:
	URenderer* Renderer;
	ID3D11Buffer* VertexBuffer;
	uint32 NumVertices;

};