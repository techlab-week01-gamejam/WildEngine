#pragma once

#ifndef __ICON_FONT_INDEX__
#define __ICON_FONT_INDEX__
#define DEFAULT_FONT		0
#define	FEATHER_FONT		1
#endif // !__ICON_FONT_INDEX__

#include "Types/Types.h";

class UEditorWindow
{
public:
	virtual ~UEditorWindow() {}
	virtual void Render() = 0;
	virtual void OnResize(uint32 Width, uint32 Height) = 0;
};

