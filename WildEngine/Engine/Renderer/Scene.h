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

    // �� ������Ʈ ���
    TArray<UPrimitiveComponent*> SceneObjects;

    // ���� Ȱ��ȭ�� ī�޶�
    UCameraComponent* ActiveCamera;

};