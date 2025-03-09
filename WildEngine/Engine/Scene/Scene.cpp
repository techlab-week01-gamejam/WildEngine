#include "Scene.h"

#include "d3d11.h"
#include "DirectXMath.h"

#include "Renderer/URenderer.h"

#include "Components/CameraComponent.h"
#include "Components/CubeComponent.h"

#include "Math/Matrix.h"
#include "Types/CommonTypes.h"
#include "Object/ObjectManager.h"
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
    UObjectManager& ObjManager = UObjectManager::GetInstance();
    UObjectFactory& ObjFactory = UObjectFactory::GetInsantce();

    // Camera 설정
    if (PrimaryCamera == nullptr)
    {
        //PrimaryCamera = static_cast<UCameraComponent*>(ObjFactory.ConstructObject(UCameraComponent::GetClass()));
        PrimaryCamera = new UCameraComponent();
    }

    // Test Cube
    if (Cube1 == nullptr)
    {
        //Cube1 = static_cast<UCubeComponent*>(ObjFactory.ConstructObject(UCubeComponent::GetClass()));
        Cube1 = new UCubeComponent(Renderer);
    }

    // 월드 행렬 초기화
    WorldMatrix = FMatrix::Identity();

    // Projection Init
    CreateProjectionView();

    // Ortho
    OrthoMatrix = FMatrix::Identity(); // XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

void UScene::CreateProjectionView()
{
    D3D11_VIEWPORT ViewPort = Renderer->ViewportInfo;
    float ScreenAspect = ViewPort.Width / ViewPort.Height; // 화면 비율 ex 1280x1080 = 1.18...
    float FarZ = PrimaryCamera->FarZ;
    float NearZ = PrimaryCamera->NearZ;

    float SinFov;
    float CosFov;
    DirectX::XMScalarSinCos(&SinFov, &CosFov, 0.5 * PrimaryCamera->FieldOfView);

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

    ProjectionMatrix = FinalProjectionMatrix;
}

void UScene::Render()
{
    // 카메라 위치에서 뷰 행렬 생성
    PrimaryCamera->Render();

    // 뷰 행렬 가져오기
    PrimaryCamera->GetViewMatrix(ViewMatrix);

    // 셰이더 상수 버퍼 업데이트
    Cube1->Render(WorldMatrix, ViewMatrix, ProjectionMatrix);
}


