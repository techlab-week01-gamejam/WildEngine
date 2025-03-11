#pragma once

// Interface
#include "Interface/ISwitchable.h"

#include "Types/Types.h"
#include "EditorWindow.h"
#include <memory>
#include "../../ImGui/imgui_impl_dx11.h"

class UEditorDesigner
{
public:
	static UEditorDesigner& Get()
	{
		static UEditorDesigner Instance;
		return Instance;
	}

	void AddWindow(const FString& WindowId, std::shared_ptr<UEditorWindow> Window)
	{
		Windows[WindowId] = Window;
	}
	
	/**
	    ĳ������ ���� Window�� �����ɴϴ�.
		���� setter �� ���� ���� ������ �� �ֽ��ϴ�.
		auto window = UEditorDesigner::Get().GetWindow("mainWindow");
		if (window) {
			// dynamic_cast�� ���� MyWindow Ÿ������ ��ȯ �� setter ȣ��
			if (MyWindow* mw = dynamic_cast<MyWindow*>(window.get())) {
				mw->SetValue(42);
			}
		}
	
	*/
	// ���� �����ǰ� �ִ� ������ â�� �����ɴϴ�.
	std::shared_ptr<UEditorWindow> GetWindow(const FString WindowId)
	{
		auto it = Windows.find(WindowId);
		if (it != Windows.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void OnResize(UINT32 Width, UINT32 Height)
	{
		bFinishClearWindows = false;
		Windows.clear();
		bFinishClearWindows = true;
	}

	void Render()
	{
		if (Windows.empty()) return;

		for ( TPair<FString, std::shared_ptr<UEditorWindow>> Window : Windows)
		{
			Window.second->Render();
		}
	}
	bool IsClear()
	{
		return bFinishClearWindows;
	}

	void Clear()
	{
		bFinishClearWindows = false;
	}

private:
	UEditorDesigner() = default;
	~UEditorDesigner() = default;
	UEditorDesigner(const UEditorDesigner&) = delete;
	UEditorDesigner& operator=(const UEditorDesigner&) = delete;

	TMap<FString, std::shared_ptr<UEditorWindow>> Windows;

	bool bFinishClearWindows;
};