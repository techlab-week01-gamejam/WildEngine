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

USphereComponent::USphereComponent(const USphereComponent&)
{
}

USphereComponent::USphereComponent(URenderer* InRenderer, FVector Location)
{
    RelativeLocation = Location;
}

USphereComponent::USphereComponent(URenderer* InRenderer, FVector Location, FVector Rotation)
{
    RelativeLocation = Location;
    RelativeRotation = Rotation;
}

USphereComponent::USphereComponent(URenderer* InRenderer, FVector Location, FVector Rotation, FVector Scale)
{
    RelativeLocation = Location;
    RelativeRotation = Rotation;
    RelativeScale3D = Scale;
}

void USphereComponent::Initialize()
{
    NumVertices = sizeof(cube_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));

    RelativeLocation.X = 0.0f;
    RelativeLocation.Y = 0.0f;
    RelativeLocation.Z = 3.0f;
}

USphereComponent::~USphereComponent()
{
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

//UClass* USphereComponent::GetClass()
//{
//    static UClass SphereClass("USphereComponent", []() -> UObject* { return new USphereComponent(); });
//    return &SphereClass;
//}

void USphereComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
    if (rot == 180) rot = 0;
    rot += 0.01f;

    FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
    FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(rot, 0, rot);
    FMatrix Scaling = FMatrix::Scaling(1, 1, 1);

    FMatrix World = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(World, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);

}


