#include "Scene.h"

#include "d3d11.h"
#include "DirectXMath.h"

#include "Renderer/URenderer.h"

#include "Components/CameraComponent.h"
#include "Components/CubeComponent.h"

#include "Math/Matrix.h"
#include "Types/CommonTypes.h"

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
    // Camera ����
    if (PrimaryCamera == nullptr)
    {
        PrimaryCamera = new UCameraComponent();
    }
	PrimaryCamera->SetViewportSize(Renderer->ViewportInfo.Width, Renderer->ViewportInfo.Height);

    // Test Cube
    if (Cube1 == nullptr)
    {
        Cube1 = new UCubeComponent(Renderer);
    }

    // ���� ��� �ʱ�ȭ
    WorldMatrix = FMatrix::Identity();

    // Projection Init
    CreateProjectionView();

    // Ortho
    OrthoMatrix = FMatrix::Identity(); // XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

void UScene::CreateProjectionView()
{
    D3D11_VIEWPORT ViewPort = Renderer->ViewportInfo;
    float ScreenAspect = ViewPort.Width / ViewPort.Height; // ȭ�� ���� ex 1280x1080 = 1.18...
    float FarZ = PrimaryCamera->FarZ;
    float NearZ = PrimaryCamera->NearZ;

    float SinFov;
    float CosFov;
    float FovRadian = DirectX::XMConvertToRadians(PrimaryCamera->FieldOfView);
    DirectX::XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovRadian);

    // 0~1 ������ ����ȭ�� ������ Height�� width�� ǥ���Ǿ����
    float Height = CosFov / SinFov; // tan(0.5*FOV);
    float Width = Height / ScreenAspect; // ��Ⱦ�� ���� ������
    float fRange = FarZ / (FarZ - NearZ); // z �� ���� : near �� far clip plane�� ���̿� ���� z �� ������ ��

    float Projection[4][4] = {
        { Width, 0, 0, 0 },
        { 0, Height, 0, 0},
        { 0, 0, fRange, 1},
        { 0, 0, -fRange * NearZ, 0} // ��ü�� z ��ǥ ���� �� �ùٸ� ���� �� ����
    };

    FMatrix FinalProjectionMatrix(Projection);

    ProjectionMatrix = FinalProjectionMatrix;
}

void UScene::Render()
{
    // ī�޶� ��ġ���� �� ��� ����
    PrimaryCamera->Render();

    // �� ��� ��������
    PrimaryCamera->GetViewMatrix(ViewMatrix);

    // ���̴� ��� ���� ������Ʈ
    Cube1->Render(WorldMatrix, ViewMatrix, ProjectionMatrix);
}


