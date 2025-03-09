#pragma once

#include "Windows.h"
#include "Types/Types.h"

class UWildEditor
{
public:
	UWildEditor();
	UWildEditor(const UWildEditor&);
	~UWildEditor();

	void Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND hWnd);
	void Release();

	void Render();

private:
	void PreferenceStyle();

	void CreateUsingFont();
};

