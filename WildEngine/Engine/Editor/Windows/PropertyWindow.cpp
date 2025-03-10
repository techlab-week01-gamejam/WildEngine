#include "PropertyWindow.h"
#include "../../../ImGui/imgui.h"
#include "Object/Object.h"
#include "Scene/Scene.h"

#include <Components/PrimitiveComponent.h>

PropertyWindow::PropertyWindow()
{
	bIsFocused = true;

	Translation.Set(0,0,0);
	Rotation.Set(0,0,0);
	Scale.Set(0,0,0);

	ObjectUUID = -1;
}

void PropertyWindow::Render()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 270, 10));
	ImGui::SetNextWindowSize(ImVec2(260, 280));
	ImGui::Begin("Property Panel", nullptr, ImGuiWindowFlags_NoResize);

	memcpy(ObjectTranslation, &Translation, sizeof(ObjectTranslation));
	memcpy(ObjectRotation, &Rotation, sizeof(ObjectRotation));
	memcpy(ObjectScale, &Scale, sizeof(Scale));

	if (bIsFocused)
	{
		ImGui::DragFloat3("Location", ObjectTranslation);
		ImGui::DragFloat3("Rotation", ObjectRotation);
		ImGui::DragFloat3("Scale", ObjectScale);

		ImGui::Text("GUID : %d", ObjectUUID);
	}

	ImGui::Separator(); // 수평 구분선
	ImGui::End();
}

FVector PropertyWindow::GetLocation()
{
	return FVector(ObjectTranslation[0], ObjectTranslation[1], ObjectTranslation[2]);
}

FVector PropertyWindow::GetRotation()
{
	return FVector(ObjectRotation[0], ObjectRotation[1], ObjectRotation[2]);
}

FVector PropertyWindow::GetScale()
{
	return FVector(ObjectScale[0], ObjectScale[1], ObjectScale[2]);
}

void PropertyWindow::SetLocation(FVector& Vector)
{
	Translation = Vector;
}

void PropertyWindow::SetRotation(FVector& Vector)
{
	Rotation = Vector;
}

void PropertyWindow::SetScale(FVector& Vector)
{
	Scale = Vector;
}

void PropertyWindow::SetUUID(uint32 UUID)
{
	ObjectUUID = UUID;
}
