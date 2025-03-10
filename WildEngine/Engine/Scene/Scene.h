#pragma once

#include "Math/Matrix.h"

class URenderer;
class UCameraComponent;
class UCubeComponent;
class UGizmoComponent;

class UScene
{
public:
	UScene(URenderer* InRenderer);
	UScene(const UScene&);
	~UScene();

	void Render();
	URenderer* GetRenderer() const
	{
		return Renderer;
	};

private:
	void Initialize();
	void CreateProjectionView();

private:
	URenderer* Renderer = nullptr;
	UCameraComponent* PrimaryCamera = nullptr;
	UCubeComponent* Cube1 = nullptr;
	UGizmoComponent* SceneGizmo = nullptr;

	FMatrix WorldMatrix;
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix OrthoMatrix;
};
