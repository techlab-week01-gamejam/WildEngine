#include "Scene.h"

UScene::UScene()
{
    ActiveCamera = nullptr;
}

void UScene::AddObject(UPrimitiveComponent* Obj)
{
	SceneObjects.push_back(Obj);
}

void UScene::SetActiveCamera(UCameraComponent* Camera)
{
    ActiveCamera = Camera;
}

void UScene::CalculateMVP()
{
    if (!ActiveCamera)
        return;

    float AspectRatio = 16.0f / 9.0f; // 창 크기 기반으로 설정 가능

    FMatrix ViewMatrix = ActiveCamera->GetViewMatrix();
    FMatrix ProjectionMatrix = ActiveCamera->GetProjectionMatrix(AspectRatio);

    for (UPrimitiveComponent* Obj : SceneObjects)
    {
        FMatrix ModelMatrix = Obj->GetModelMatrix();
        FMatrix temp = ModelMatrix * ViewMatrix;
        FMatrix MVP = ModelMatrix * ViewMatrix * ProjectionMatrix;

        Obj->SetMVPMatrix(MVP); // 최종 행렬을 개별 오브젝트에 저장
    }
}
