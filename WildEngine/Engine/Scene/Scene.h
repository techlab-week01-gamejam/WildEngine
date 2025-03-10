#pragma once

#include "Math/Matrix.h"
#include "Interface/IScene.h"

class URenderer;
class UObject;
class UCameraComponent;
class UCubeComponent;

class UScene : public IScene
{
public:
	UScene(URenderer* InRenderer);
	UScene(const UScene&);
	~UScene();

	void Render();

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

private:
	void Initialize();
	FMatrix CreateProjectionView();
	FMatrix CreateOrthogonalView();

private:
	URenderer* Renderer = nullptr;
	UCameraComponent* PrimaryCamera = nullptr;
	UCubeComponent* Cube1 = nullptr;

	UObject* SelectedObject = nullptr;

	FMatrix WorldMatrix;
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix OrthoMatrix;
};
