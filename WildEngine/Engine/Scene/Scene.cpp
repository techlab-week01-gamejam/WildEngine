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

    // Test Cube
    if (Cube1 == nullptr)
    {
        Cube1 = new UCubeComponent(Renderer);
    }

    // ���� ��� �ʱ�ȭ
    WorldMatrix = FMatrix::Identity();

    // Projection Init
    ProjectionMatrix = CreateProjectionView();

    // Ortho
    OrthoMatrix = CreateOrthogonalView(); // XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

FMatrix UScene::CreateProjectionView()
{
    D3D11_VIEWPORT ViewPort = Renderer->ViewportInfo;
    float ScreenAspect = ViewPort.Width / ViewPort.Height; // ȭ�� ���� ex 1280x1080 = 1.18...
    float FarZ = PrimaryCamera->FarZ;
    float NearZ = PrimaryCamera->NearZ;

    float SinFov;
    float CosFov;
    DirectX::XMScalarSinCos(&SinFov, &CosFov, 0.5 * PrimaryCamera->FieldOfView);

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
        { 0.0f,         0.0f,         -NearZ / (FarZ - NearZ),  1.0f }
    };

    FMatrix FinalOrthoMatrix(Projection);

    return FinalOrthoMatrix;
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


