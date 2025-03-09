#pragma once

#include "Math/Matrix.h"

class URenderer;
class UCameraComponent;
class UCubeComponent;
class USphereComponent;

class UScene
{
public:
	UScene(URenderer* InRenderer);
	UScene(const UScene&);
	~UScene();

	void Render();

private:
	void Initialize();
	void CreateProjectionView();

private:
	URenderer* Renderer = nullptr;
	UCameraComponent* PrimaryCamera = nullptr;
	UCubeComponent* Cube1 = nullptr;
	USphereComponent* Sphere1 = nullptr;

	FMatrix WorldMatrix;
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix OrthoMatrix;
};
