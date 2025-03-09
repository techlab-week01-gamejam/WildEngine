#include "CubeComponent.h"
#include "Cube/Cube.h"


UCubeComponent::UCubeComponent(URenderer* InRenderer)
{
    // �⺻ ��ġ ����
    RelativeLocation = FVector(0.0f, 0.0f, 3.0f);

    // �⺻ ȸ�� ����
    RelativeRotation = FVector(0.0f, 0.0f, 0.0f);

    // �⺻ ũ�� ���� (1.0, 1.0, 1.0)
    RelativeScale3D = FVector(1.0f, 1.0f, 1.0f);

    Renderer = InRenderer;    
    NumVertices = sizeof(cube_vertices) / sizeof(FVertexSimple);
    VertexBuffer = nullptr;
    InitializeMesh();
}

void UCubeComponent::InitializeMesh() {
    VertexBuffer = Renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
}

void UCubeComponent::Render()
{
	FMatrix ModelMatrix = GetModelMatrix();

    if (nullptr == VertexBuffer)
        return;

    Renderer->RenderPrimitive(VertexBuffer, sizeof(cube_vertices) / sizeof(FVertexSimple));
}
