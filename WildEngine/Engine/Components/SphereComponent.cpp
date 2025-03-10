#include "SphereComponent.h"
#include "Renderer/URenderer.h"

USphereComponent::USphereComponent()
{
}
USphereComponent::USphereComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}

USphereComponent::USphereComponent(URenderer* InRenderer, const FVector& InLocation)
{
    Renderer = InRenderer;
    RelativeLocation = InLocation;
    Initialize();
}
USphereComponent::USphereComponent(const USphereComponent&)
{
}
USphereComponent::~USphereComponent()
{
    // �ʿ��� ��� ���ҽ� ���� �ڵ� �߰�
}

UClass* USphereComponent::GetClass()
{
    static UClass SphereClass("USphereComponent", UObject::GetClass());
    return &SphereClass;
}

UClass* USphereComponent::GetInstanceClass() const
{
    return GetClass();
}
void USphereComponent::Initialize()
{
    NumVertices = sizeof(sphere_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
}