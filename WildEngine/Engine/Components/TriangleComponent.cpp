#include "TriangleComponent.h"
#include "Renderer/URenderer.h"

UTriangleComponent::UTriangleComponent()
{
}
UTriangleComponent::UTriangleComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}
UTriangleComponent::UTriangleComponent(URenderer* InRenderer, const FVector& InLocation)
{
    Renderer = InRenderer;
    RelativeLocation = InLocation;
    Initialize();
}
UTriangleComponent::UTriangleComponent(const UTriangleComponent&)
{
}
UTriangleComponent::~UTriangleComponent()
{
}

UClass* UTriangleComponent::GetClass()
{
    static UClass TriangleClass("UTriangleComponent", UObject::GetClass());
    return &TriangleClass;
}

UClass* UTriangleComponent::GetInstanceClass() const
{
    return GetClass();
}

bool UTriangleComponent::CheckRayIntersection(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult)
{
    return false;
}

void UTriangleComponent::Initialize()
{
    NumVertices = sizeof(triangle_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
}