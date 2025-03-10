#include "PropertyWindow.h"
#include "../../../ImGui/imgui.h"
#include "Object/Object.h"
#include "Scene/Scene.h"

#include <Components/PrimitiveComponent.h>

PropertyWindow::PropertyWindow()
{
	bIsFocused = true;

	Translation = nullptr;
	Rotation = nullptr;
	Scale = nullptr;

	ObjectUUID = -1;
}

void PropertyWindow::Render()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 270, 10));
	ImGui::SetNextWindowSize(ImVec2(260, 280));
	ImGui::Begin("Property Panel", nullptr, ImGuiWindowFlags_NoResize);

	if (bIsFocused)
	{
		UpdateVectorToFloat(Translation, ObjectTranslation);

		if (ImGui::DragFloat3("Location", ObjectTranslation))
		{
			Translation->X = ObjectTranslation[0];
			Translation->Y = ObjectTranslation[1];
			Translation->Z = ObjectTranslation[2];
		}
		
		UpdateVectorToFloat(Rotation, ObjectRotation);
		if (ImGui::DragFloat3("Rotation", ObjectRotation))
		{
			Rotation->X = ObjectRotation[0];
			Rotation->Y = ObjectRotation[1];
			Rotation->Z = ObjectRotation[2];
		}

		UpdateVectorToFloat(Scale, ObjectScale);
		if (ImGui::DragFloat3("Scale", ObjectScale))
		{
			Scale->X = ObjectScale[0];
			Scale->Y = ObjectScale[1];
			Scale->Z = ObjectScale[2];
		}

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
	Translation = &Vector;
}

void PropertyWindow::SetRotation(FVector& Vector)
{
	Rotation = &Vector;
}

void PropertyWindow::SetScale(FVector& Vector)
{
	Scale = &Vector;
}

void PropertyWindow::SetUUID(uint32 UUID)
{
	ObjectUUID = UUID;
}

void PropertyWindow::UpdateVectorToFloat(FVector* v, float f[])
{
	f[0] = v->X;
	f[1] = v->Y;
	f[2] = v->Z;
}