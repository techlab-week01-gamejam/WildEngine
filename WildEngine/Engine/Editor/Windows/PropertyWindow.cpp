#include "PropertyWindow.h"
#include "../../../ImGui/imgui.h"

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

	if (bIsFocused)
	{
		if (ImGui::DragFloat3("Location", ObjectTranslation))
		{
			Translation.X = ObjectTranslation[0];
			Translation.Y = ObjectTranslation[1];
			Translation.Z = ObjectTranslation[2];
		}

		if (ImGui::DragFloat3("Rotation", ObjectRotation))
		{
			Rotation.X = ObjectRotation[0];
			Rotation.Y = ObjectRotation[1];
			Rotation.Z = ObjectRotation[2];
		}

		if (ImGui::DragFloat3("Scale", ObjectScale))
		{
			Scale.X = ObjectScale[0];
			Scale.Y = ObjectScale[1];
			Scale.Z = ObjectScale[2];
		}

		ImGui::Text("GUID : %d", ObjectUUID);
	}

	ImGui::Separator(); // 수평 구분선
	ImGui::End();
}
