#pragma once
#include "Interface/ISingleton.h"
#include "Types/Types.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
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

	// ���콺 �Է� ���� �Լ� �߰�
	void ProcessMouseMovement(int CurrentX, int CurrentY);
	void ResetMouseDeltas();
	float GetMouseDeltaX() const
	{
		return MouseDeltaX;
	}
	float GetMouseDeltaY() const
	{
		return MouseDeltaY;
	}
	int GetMouseX() const
	{
		return LastMouseX;
	}
	int GetMouseY() const
	{
		return LastMouseY;
	}

private:
	friend class ISingleton<FInputManager>;
	FInputManager() = default;

	TMap<int, EKeyState> KeyStateMap;

	int LastMouseX = 0;
	int LastMouseY = 0;
	float MouseDeltaX = 0.0f;
	float MouseDeltaY = 0.0f;
};

