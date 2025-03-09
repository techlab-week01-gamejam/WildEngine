#pragma once
#include "Components/PrimitiveComponent.h"
#include "Components/CameraComponent.h"

class URenderer;

class UScene {
public:
    UScene();

    void AddObject(UPrimitiveComponent* Obj);
    void SetActiveCamera(UCameraComponent* Camera);
    void CalculateMVP();

    // 씬 오브젝트 목록
    TArray<UPrimitiveComponent*> SceneObjects;

    // 현재 활성화된 카메라
    UCameraComponent* ActiveCamera;

};