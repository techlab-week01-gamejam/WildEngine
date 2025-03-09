#pragma once

#include "Components/SceneComponent.h"

class UCameraComponent : public USceneComponent
{
public:
	UCameraComponent();
	UCameraComponent(const UCameraComponent&);
	~UCameraComponent();

	static UClass* GetClass();

	void Render();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	FVector GetPosition();
	FVector GetRotation();

	void GetViewMatrix(FMatrix& InViewMatrix);

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

	// Camera ViewMatrix
	FMatrix ViewMatrix;
};

