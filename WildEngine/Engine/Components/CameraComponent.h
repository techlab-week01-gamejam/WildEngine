#pragma once

#include "Components/SceneComponent.h"
#include "Input/InputManager.h"

class UCameraComponent : public USceneComponent
{
public:
	UCameraComponent();
	UCameraComponent(const UCameraComponent&);
	~UCameraComponent();

	void Render();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	FVector GetPosition();
	FVector GetRotation();

	void GetViewMatrix(FMatrix& InViewMatrix);

	// 마우스로 회전 처리
	void UpdateRotationFromMouse();

public:
	// Default FOV : 90
	float FieldOfView;

	// Default Near : 0.1
	float NearZ;

	// Default Far : 1000
	float FarZ;

private:
	void Initialize();

	FMatrix CreateLookAt();


private:
	// Camera Position
	FVector CameraPosition;

	// Camera Rotation
	FVector CameraRotation;

	// Camera Target
	//FVector CameraTarget;

	// Camera ViewMatrix
	FMatrix ViewMatrix;
	FMatrix RotationMatrix;
};

