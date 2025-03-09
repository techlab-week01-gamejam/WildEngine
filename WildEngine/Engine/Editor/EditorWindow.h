#pragma once

#define DEFAULT_FONT		0
#define	FEATHER_FONT		1

class UEditorWindow
{
public:
	virtual ~UEditorWindow() {}
	virtual void Render() = 0;
};

