#pragma once

#include "Windows.h"
#include "Types/Types.h"


class URenderer;

class UWildEditor
{
public:
	UWildEditor(URenderer* InRenderer);
	UWildEditor(const UWildEditor&);
	~UWildEditor();

	void Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND hWnd);
	void Release();

	void Render();

private:
	void PreferenceStyle();

	void CreateUsingFont();

private:
	URenderer* Renderer;
};

