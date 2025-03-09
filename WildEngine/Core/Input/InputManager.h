#pragma once
#include "Interface/ISingleton.h"
#include <unordered_map>
#include <Windows.h>

enum class EKeyState
{
	None,		// �Է� ����
	Pressed,	// ó�� ���� ���� (1������ ���ȸ� ����)
	Held,		// ������ �ִ� ���� (Pressed ����)
	Released	// Ű���� ���� �� ���� (1������ ���ȸ� ����)
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

