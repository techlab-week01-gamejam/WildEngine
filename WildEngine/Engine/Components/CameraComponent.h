#pragma once

#include "Components/SceneComponent.h"
#include "Input/InputManager.h"

class UCameraComponent : public USceneComponent
{
public:
	UCameraComponent();
	UCameraComponent(const UCameraComponent&);
	~UCameraComponent();

	static UClass* GetClass();
	UClass* GetInstanceClass() const override;

	void Render();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	// ����Ʈ ũ�� ����
	void SetViewportSize(float Width, float Height);

	FVector GetPosition();
	FVector GetRotation();

	void GetViewMatrix(FMatrix& InViewMatrix);

	// ���콺�� ȸ�� ó��
	void UpdateRotationFromMouse();

	// Ray Casting �Լ� �߰�
	FVector GetRayDirection(int ScreenX, int ScreenY);

public:
	// Default FOV : 90
	float FieldOfView;

	// Default Near : 0.1
	float NearZ;

	// Default Far : 1000
	float FarZ;

	// Perspective or Orthogonal
	bool bIsOrthogonal;

	// Camera Position
	FVector CameraPosition;

	// Camera Rotation
	FVector CameraRotation;
private:
	void Initialize();

	FMatrix CreateLookAt();


private:
	// Camera Target
	//FVector CameraTarget;

	// Camera ViewMatrix
	FMatrix ViewMatrix;
	FMatrix RotationMatrix;

	// Viewport ũ��
	float ViewportWidth;
	float ViewportHeight;
};

