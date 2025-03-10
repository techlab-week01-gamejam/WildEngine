#include "Scene.h"

#include "d3d11.h"
#include "DirectXMath.h"

#include "Renderer/URenderer.h"

#include "Components/CameraComponent.h"
#include "Components/CubeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/GizmoComponent.h"

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
        PrimaryCamera = new UCameraComponent();
    }
    PrimaryCamera->SetViewportSize(Renderer->ViewportInfo.Width, Renderer->ViewportInfo.Height);

    // Test Cube
    if (Cube1 == nullptr)
    {
        Cube1 = new UCubeComponent(Renderer);
		Cube1->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
		Cube1->RelativeRotation = FVector(0.0f, 0.0f, 0.0f);
		Cube1->RelativeScale3D = FVector(1.0f, 1.0f, 1.0f);
    }

	// Test Sphere
    if (Sphere1 == nullptr)
    {
		Sphere1 = new USphereComponent(Renderer);
		Sphere1->RelativeLocation = FVector(5.0f, 0.0f, 0.0f);
        Sphere1->RelativeRotation = FVector(0.f, 0.f, 0.f);
        Sphere1->RelativeScale3D = FVector(1.f, 1.f, 1.f);
    }
    
    
    if (SceneGizmo == nullptr)
    {
        SceneGizmo = new UGizmoComponent(Renderer);
    }


    // 월드 행렬 초기화
    WorldMatrix = FMatrix::Identity();

    // Projection Init
    ProjectionMatrix = CreateProjectionView();
    // 카메라에 투영 행렬 전달
	PrimaryCamera->SetProjectionMatrix(ProjectionMatrix);

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
        { 0.0f,         0.0f,         -NearZ / (FarZ - NearZ),  1.0f }
    };

    FMatrix FinalOrthoMatrix(Projection);

    return FinalOrthoMatrix;
}

// 모든 Primitive들과 Ray의 충돌을 검사
bool UScene::RayCast(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult)
{
    bool bHasHit = false;
    float MinDistance = FLT_MAX;

	OutHitResult = FHitResult();

    // 모든 Primitive를 순회하며 충돌 검사
    UPrimitiveComponent* Primitives[] = { Cube1, Sphere1 };
    for (UPrimitiveComponent* Primitive : Primitives)
    {
        FHitResult TempHit;
        if (Primitive->CheckRayIntersection(RayOrigin, RayDirection, TempHit))
        {
            if (TempHit.Distance < MinDistance)
            {
                MinDistance = TempHit.Distance;
                OutHitResult = TempHit;
                bHasHit = true;
            }
        }
    }
    return bHasHit;

  //  // 모든 Primitive를 순회하며 충돌 검사
  //  
  //  for ()
  //  {

  //      // 오브젝트 타입검사 , PrimitiveComponent가 아닌 경우 스킵
  //      UPrimitiveComponent* Primitive = dynamic_cast<UPrimitiveComponent*>(Object);
  //      if (!Primitive)
  //      {
  //          continue;
  //      }

  //      FHitResult TempHit;
		//if (Primitive->CheckRayIntersection(RayOrigin, RayDirection, TempHit))
		//{
		//	if (TempHit.Distance < MinDistance)
		//	{
		//		MinDistance = TempHit.Distance;
		//		OutHitResult = TempHit;
		//		bHasHit = true;
		//	}
		//}
  //  }
  //  return bHasHit;
}

void UScene::Render()
{
    // 카메라 위치에서 뷰 행렬 생성
    PrimaryCamera->Render();

    // 뷰 행렬 가져오기
    PrimaryCamera->GetViewMatrix(ViewMatrix);

    // 셰이더 상수 버퍼 업데이트
    Cube1->Render(Cube1->GetWorldTransform(), ViewMatrix, ProjectionMatrix);

    // Test Sphere
	Sphere1->Render(Sphere1->GetWorldTransform(), ViewMatrix, ProjectionMatrix);

    // 마우스 클릭시 오브젝트 선택
	if (FInputManager::GetInst().GetKey(VK_LBUTTON) == EKeyState::Pressed ||
        FInputManager::GetInst().GetKey(VK_LBUTTON) == EKeyState::Held)
    {
        OnMouseClink(FInputManager::GetInst().GetMouseX(), FInputManager::GetInst().GetMouseY());
    }
	



    // 선택된 오브젝트가 있는 경우 기즈모 렌더링
    if (nullptr != SelectedObject)
    {
        USceneComponent* SceneComponent = dynamic_cast<USceneComponent*>(SelectedObject);
		if (SceneComponent)
		{
            FMatrix GizmoWorldMatrix = SceneComponent->GetWorldTransform();
			SceneGizmo->Render(GizmoWorldMatrix, ViewMatrix, ProjectionMatrix);
		}
    }

}

UObject* UScene::GetSelectedObject()
{
    return SelectedObject;
}

void UScene::SetSelectedObject(UObject* newSelectObject)
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

void UScene::CreateNewObject(UObject* newObject)
{
}

void UScene::OnMouseClink(int32 ScreenX, int32 ScreenY)
{
	FVector RayOrigin = PrimaryCamera->GetPosition();
	FVector RayDirection = PrimaryCamera->GetRayDirection(ScreenX, ScreenY);

	FHitResult HitResult;
	if (RayCast(RayOrigin, RayDirection, HitResult))
	{
		// 선택된 오브젝트 설정
		SelectedObject = HitResult.HitObject;
	}
	else
	{
		SelectedObject = SelectedObject;
	}
}


