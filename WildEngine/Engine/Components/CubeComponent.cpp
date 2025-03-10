#include "Components/CubeComponent.h"
#include "Renderer/URenderer.h"

UCubeComponent::UCubeComponent()
{
}
UCubeComponent::UCubeComponent(URenderer* InRenderer, FVector InLocation)
{
    Renderer = InRenderer;
    RelativeLocation = InLocation;
    Initialize();
}
UCubeComponent::UCubeComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}
UCubeComponent::UCubeComponent(const UCubeComponent&)
{
}
UCubeComponent::~UCubeComponent()
{
    // �ʿ��� ��� ���ҽ� ���� �ڵ� �߰�
}
void UCubeComponent::Initialize()
{
    NumVertices = sizeof(cube_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
}

UClass* UCubeComponent::GetClass()
{
    static UClass CubeClass("UCubeComponent", UObject::GetClass());
    return &CubeClass;
}

UClass* UCubeComponent::GetInstanceClass() const
{
    return GetClass();
}