#include "Components/CubeComponent.h"

#include "Renderer/URenderer.h"

UCubeComponent::UCubeComponent()
{
}

UCubeComponent::UCubeComponent(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}

UCubeComponent::UCubeComponent(URenderer* InRenderer, FVector Location)
{
    RelativeLocation = Location;
}

UCubeComponent::UCubeComponent(URenderer* InRenderer, FVector Location, FVector Rotation)
{
    RelativeLocation = Location;
    RelativeRotation = Rotation;
}

UCubeComponent::UCubeComponent(URenderer* InRenderer, FVector Location, FVector Rotation, FVector Scale)
{
    RelativeLocation = Location;
    RelativeRotation = Rotation;
    RelativeScale3D = Scale;
}

UCubeComponent::UCubeComponent(const UCubeComponent&)
{
}

void UCubeComponent::Initialize()
{
    NumVertices = sizeof(cube_vertices) / sizeof(FVertexType);
    VertexBuffer = Renderer->CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));

    RelativeLocation.X = 0.0f;
    RelativeLocation.Y = 0.0f;
    RelativeLocation.Z = 3.0f;
}

UCubeComponent::~UCubeComponent()
{
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

//UClass* UCubeComponent::GetClass()
//{
//    static UClass CubeClass("UCubeComponent", []() -> UObject* { return new UCubeComponent(); });
//    return &CubeClass;
//}

void UCubeComponent::Render(FMatrix WorldMatrix, FMatrix ViewMatrix, FMatrix ProjectionMatrix)
{
   /* if (rot == 180) rot = 0;
    rot += 0.01f;*/
    float Radian = 3.141592f / 180.0f;

    FMatrix Translation = FMatrix::Translation(RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z);
    FMatrix Rotation = FMatrix::CreateRotationRollPitchYaw(RelativeRotation.X * Radian, RelativeRotation.Y * Radian, RelativeRotation.Z * Radian);
    FMatrix Scaling = FMatrix::Scaling(RelativeScale3D.X, RelativeScale3D.Y, RelativeScale3D.Z);

    WorldTransform = Scaling * Rotation * Translation; // 스케일링 * 회전 * 이동

    // 셰이더 상수 버퍼 업데이트
    Renderer->UpdateShaderParameters(WorldTransform, ViewMatrix, ProjectionMatrix);

    Renderer->RenderPrimitive(VertexBuffer, NumVertices);
}


