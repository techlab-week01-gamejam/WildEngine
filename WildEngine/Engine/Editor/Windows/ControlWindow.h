#pragma once
#include "Editor/EditorWindow.h"
#include "Types/CommonTypes.h"
#include "Types/Types.h"
#include "Math/Vector.h"

#include "../ImGui/imgui.h"

class UGizmoComponent;

class ControlWindow : public UEditorWindow
{
public:
	ControlWindow();
	ControlWindow(const ControlWindow&) {};

	void Render() override;

	void SetPrimaryGizmo(UGizmoComponent* NewGizmo);
	void SetCameraLocation(FVector& Location) { CameraLocation = &Location; };
	void SetCameraRotation(FVector& Rotation) { CameraRotation = &Rotation; };
	void SetCameraFOV(float& fov) { FOV = &fov; };
	void SetOrthogonal(bool& State) { bIsOrthogonal = &State; };

private:
	bool CreateCustomInputInt(const char* label, ImGuiDataType data_type, void* p_data, const char* format, ImGuiInputTextFlags flags);

	void UpdateVectorToFloat(FVector* v, float f[]);

private:
	UINT FramePerSecond;

	int32 PrimtiveTypeNumber;
	int32 SpawnNumber;
	FString SceneName;

	bool* bIsOrthogonal;

	float* FOV;
	FVector* CameraLocation;
	FVector* CameraRotation;

	float Location[3];
	float Rotation[3];

	float WindowWidth;
	float WindowHeight;

	UGizmoComponent* PrimaryGizmo;
};

