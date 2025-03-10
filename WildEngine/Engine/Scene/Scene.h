#pragma once

#include "Math/Matrix.h"
#include "Interface/IScene.h"
#include "Types/CommonTypes.h"

class URenderer;
class UObject;
class UCameraComponent;
class UCubeComponent;
class USphereComponent;
class UGizmoComponent;

class UScene : public IScene
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

public:
	//////////////////////
	/* IScene Interface */
	//////////////////////

	/* Selected Object */
	UObject* GetSelectedObject() override;
	void SetSelectedObject(UObject* newSelectObject) override;
	bool wasSelectedObject() override;

	/* Load & Save */
	void LoadScene(void* data) override;
	void SaveScene(void* data) override;

	/* Camera */
	UCameraComponent* GetPrimaryCamera() override;

	/* Construct New Object */
	void CreateNewObject(UObject* newObject) override;

	void OnMouseClink(int32 ScreenX, int32 ScreenY);

private:
	void Initialize();
	FMatrix CreateProjectionView();
	FMatrix CreateOrthogonalView();

	bool RayCast(FVector RayOrigin, FVector RayDirection, FHitResult& OutHitResult);

private:
	URenderer* Renderer = nullptr;
	UCameraComponent* PrimaryCamera = nullptr;
	UCubeComponent* Cube1 = nullptr;
	USphereComponent* Sphere1 = nullptr;
	UGizmoComponent* SceneGizmo = nullptr;

	UObject* SelectedObject = nullptr;

	FMatrix WorldMatrix;
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix OrthoMatrix;
};
