#pragma once
#include "Interface/ISingleton.h"
#include <unordered_map>
#include <Windows.h>

enum class EKeyState
{
	None,		// 입력 없음
	Pressed,	// 처음 눌린 순간 (1프레임 동안만 유지)
	Held,		// 누르고 있는 상태 (Pressed 이후)
	Released	// 키에서 손을 뗀 순간 (1프레임 동안만 유지)
};

class FInputManager : public ISingleton<FInputManager>
{
public:
	void Init();
	void Tick(float DeltaTime);

	bool GetKeyState(int Key, EKeyState KeyState) const;
	EKeyState GetKey(int Key) const;

private:
	friend class ISingleton<FInputManager>;
	FInputManager() = default;

	std::unordered_map<int, EKeyState> KeyStateMap;
};

