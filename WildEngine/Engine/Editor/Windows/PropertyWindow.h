#pragma once
#include "Editor/EditorWindow.h"
#include "Math/Vector.h"
#include "Types/Types.h"



class PropertyWindow : public UEditorWindow
{
public:
	PropertyWindow();

	void Render() override;

private:
	bool bIsFocused;

	FVector Translation;
	FVector Rotation;
	FVector Scale;

	float ObjectTranslation[3] = {0, 0, 0};
	float ObjectRotation[3] = { 0, 0, 0 };
	float ObjectScale[3] = { 0, 0, 0 };

	INT32 ObjectUUID;
};

