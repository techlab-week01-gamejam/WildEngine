#include "Scene.h"

#include "DirectXMath.h"
#include "Renderer/URenderer.h"

#include "Components/CameraComponent.h"
#include "Components/CubeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TriangleComponent.h"
#include "Components/GizmoComponent.h"

#include "Math/Matrix.h"
#include "Types/CommonTypes.h"
#include "Object/ObjectFactory.h"

UScene::UScene(URenderer* InRenderer)
{
    Renderer = InRenderer;
    Initialize();
}

UScene::UScene(const UScene&)
{
}

UScene::~UScene()
{
    delete PrimaryCamera;
}

void UScene::Initialize()
{

    UObjectManager& ObjManager = UObjectManager::GetInst();
    UObjectFactory& ObjFactory = UObjectFactory::GetInst();

    ObjFactory.RegisterClass<UCameraComponent>();
    ObjFactory.RegisterClassWithArgs<UCubeComponent, URenderer*>();

    // Camera 설정

    if (PrimaryCamera == nullptr)
    {
        //PrimaryCamera = new UCameraComponent();
        PrimaryCamera = ObjFactory.ConstructObject<UCameraComponent>(UCameraComponent::GetClass());
        if (PrimaryCamera && PrimaryCamera->IsA(UCameraComponent::GetClass())) {
            FDebugConsole::DebugPrint("CameraComponent class selected!");
        }
    }

    // Test Cube
    if (Cube1 == nullptr)
    {
        //Cube1 = new UCubeComponent(Renderer);
        Cube1 = ObjFactory.ConstructObject<UCubeComponent>(UCubeComponent::GetClass(), Renderer);

    }

    SetSelectedObject(Cube1);
    
    if (SceneGizmo == nullptr)
    {
        SceneGizmo = new UGizmoComponent(Renderer);
    }

    CreateNewObject("cube", 2);

    // Test
    SelectedObject = Cube1;

    // 월드 행렬 초기화
    WorldMatrix = FMatrix::Identity();

    // Projection Init
    ProjectionMatrix = CreateProjectionView();

    // Ortho
    OrthoMatrix = CreateOrthogonalView(); // XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

FMatrix UScene::CreateProjectionView()
{
    D3D11_VIEWPORT ViewPort = Renderer->ViewportInfo;
    float ScreenAspect = ViewPort.Width / ViewPort.Height; // 화면 비율 ex 1280x1080 = 1.18...
    float FarZ = PrimaryCamera->FarZ;
    float NearZ = PrimaryCamera->NearZ;

    float SinFov;
    float CosFov;
    float ToRadian = DirectX::XMConvertToRadians(0.5 * PrimaryCamera->FieldOfView);
    DirectX::XMScalarSinCos(&SinFov, &CosFov, ToRadian);

    // 0~1 사이의 정규화된 값으로 Height와 width가 표현되어야함
    float Height = CosFov / SinFov; // tan(0.5*FOV);
    float Width = Height / ScreenAspect; // 종횡비 수평 스케일
    float fRange = FarZ / (FarZ - NearZ); // z 값 보정 : near 와 far clip plane의 차이에 따른 z 축 스케일 값

    float Projection[4][4] = {
        { Width, 0, 0, 0 },
        { 0, Height, 0, 0},
        { 0, 0, fRange, 1},
        { 0, 0, -fRange * NearZ, 0} // 물체의 z 좌표 투영 후 올바른 깊이 값 매핑
    };

    FMatrix FinalProjectionMatrix(Projection);

    return FinalProjectionMatrix;
}

FMatrix UScene::CreateOrthogonalView()
{
    D3D11_VIEWPORT ViewPort = Renderer->ViewportInfo;

    float Width = ViewPort.Width;
    float Height = ViewPort.Height;

    float FarZ = PrimaryCamera->FarZ;
    float NearZ = PrimaryCamera->NearZ;

    float Projection[4][4] = {
        { 2.0f / Width, 0.0f,           0.0f,                   0.0f },
        { 0.0f,         2.0f / Height,  0.0f,                   0.0f },
        { 0.0f,         0.0f,         1.0f / (FarZ - NearZ),    0.0f },
        { 0.0f,         0.0f,         NearZ / (FarZ - NearZ),   1.0f }
    };

    FMatrix FinalOrthoMatrix(Projection);

    return FinalOrthoMatrix;
}

void UScene::Render()
{
    // 카메라 위치에서 뷰 행렬 생성
    PrimaryCamera->Render();

    // 뷰 행렬 가져오기
    PrimaryCamera->GetViewMatrix(ViewMatrix);

    // 투영 행렬 가져오기 (ortho or pers)
    ProjectionMatrix = PrimaryCamera->bIsOrthogonal ? CreateOrthogonalView() : CreateProjectionView();

    // 선택된 오브젝트가 있는 경우
    if (SelectedObject)
    {
        // 기즈모 렌더링
        SceneGizmo->Render(SelectedObject->GetWorldTransform(), ViewMatrix, ProjectionMatrix);
    }

    // Object array 렌더링
    for (uint32 i = 0; i < GUObjectArray.size(); i++)
    {
        if (UPrimitiveComponent* Primitive = dynamic_cast<UPrimitiveComponent*>(GUObjectArray[i]))
        {
            if (Primitive)
            {
                Primitive->Render(WorldMatrix, ViewMatrix, ProjectionMatrix);
            }
        }
    }

}

USceneComponent* UScene::GetSelectedObject()
{
    return SelectedObject;
}

void UScene::SetSelectedObject(USceneComponent* newSelectObject)
{
    SelectedObject = newSelectObject;
}

bool UScene::wasSelectedObject()
{
    return nullptr != SelectedObject;
}

void UScene::LoadScene(void* data)
{
}

void UScene::SaveScene(void* data)
{
}

UCameraComponent* UScene::GetPrimaryCamera()
{
    return PrimaryCamera;
}

void UScene::CreateNewObject(FString ObjectType, int Count)
{
    if (Count <= 0) return;

    FVector Location;

    for (int i = 0; i < Count; ++i)
    {
        if (!ObjectType.compare("cube"))
        {
            new UCubeComponent(Renderer, Location);
        }
        else if (!ObjectType.compare("sphere"))
        {
            new USphereComponent(Renderer, Location);
        }
        else if (!ObjectType.compare("triangle"))
        {
            new UTriangleComponent(Renderer, Location);
        }

        Location.X += 1;
    }
}


