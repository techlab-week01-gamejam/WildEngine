#include "CameraComponent.h"
#include "Input/InputManager.h"
#include "DirectXMath.h"
#include "Math/Matrix.h"

UCameraComponent::UCameraComponent()
{
	Initialize();
}

UCameraComponent::UCameraComponent(const UCameraComponent&)
{
}

UCameraComponent::~UCameraComponent()
{
}

void UCameraComponent::Initialize()
{
	FieldOfView = 60.f;

	NearZ = 0.1f;
	FarZ = 1000.0f;

	CameraPosition.X = 0.0f;
	CameraPosition.Y = 0.0f;
	CameraPosition.Z = -5.0f;

	CameraRotation.X = 0.0;
	CameraRotation.Y = 0.0;
	CameraRotation.Z = 0.0;

	// ��ǥ���� ���� ��ǥ�� ������ ��ġ�� ����
	//CameraTarget = FVector(0.0f, 0.0f, 0.0f);
}

FMatrix UCameraComponent::CreateLookAt()
{
	// 1. ī�޶� ȸ�� ��� ���� (�Է� ������ ���� ����)
	RotationMatrix = FMatrix::CreateRotationRollPitchYaw(CameraRotation.X, CameraRotation.Y, CameraRotation.Z);

	// 2. ȸ�� ����� ����� �⺻ forward�� up ���� ��ȯ
	//    (LH ��ǥ�迡�� �⺻ forward = (0, 0, 1), up = (0, 1, 0))
	FVector Forward = RotationMatrix * FVector(0.0f, 0.0f, 1.0f);
	FVector Up = RotationMatrix * FVector(0.0f, 1.0f, 0.0f);

	// 3. LookAt ��ǥ ���: ī�޶� ��ġ���� Forward �������� �� ���� �̵�
	// -> ��ǥ���� CameraTarget�� ����Ͽ� ������ ��ǥ�� ����
	FVector LookAt = CameraPosition + Forward;

	// 4. �������� ��ǥ�� ����
	//    ForwardNormalized�� ī�޶� �ٶ󺸴� ���� ����
	FVector ForwardNormalized = (LookAt - CameraPosition).Normalized();
	// LH ��ǥ��: Right = normalize(cross(Up, ForwardNormalized))
	FVector Right = Up.Cross(ForwardNormalized).Normalized();
	// ������ Up ����: NewUp = cross(ForwardNormalized, Right)
	FVector NewUp = ForwardNormalized.Cross(Right);

	// 5. ���� �̵� ��� ���: �� �� ���Ϳ� ī�޶� ��ġ�� ������ ����
	float D0 = -Right.Dot(CameraPosition);
	float D1 = -NewUp.Dot(CameraPosition);
	float D2 = -ForwardNormalized.Dot(CameraPosition);

	// 6. �� ��� ���� (row-major ���)
	/*float m[4][4] = {
		{ Right.X,         Right.Y,         Right.Z,         D0 },
		{ NewUp.X,         NewUp.Y,         NewUp.Z,         D1 },
		{ ForwardNormalized.X, ForwardNormalized.Y, ForwardNormalized.Z, D2 },
		{ 0.0f,            0.0f,            0.0f,            1.0f }
	};*/

	float m[4][4] = {
		{ Right.X,		NewUp.X,    ForwardNormalized.X,	0.0f },
		{ Right.Y,		NewUp.Y,    ForwardNormalized.Y,	0.0f },
		{ Right.Z,		NewUp.Z,    ForwardNormalized.Z,	0.0f },
		{	   D0,           D1,				     D2,	1.0f }
	};


	return FMatrix(m);
}

void UCameraComponent::Render()
{
	// ���콺 �̵����� �̿��� ȸ�� ó��
	UpdateRotationFromMouse();

	float MoveSpeed = 0.3f;

	// Ű���� �Է� ó��
	if(FInputManager::GetInst().GetKey('W') == EKeyState::Held)
	{
		CameraPosition += RotationMatrix * FVector(0.0f, 0.0f, MoveSpeed);
	}
	if (FInputManager::GetInst().GetKey('A') == EKeyState::Held)
	{
		CameraPosition -= RotationMatrix * FVector(MoveSpeed, 0.0f, 0.0f);
	}
	if (FInputManager::GetInst().GetKey('S') == EKeyState::Held)
	{
		CameraPosition -= RotationMatrix * FVector(0.0f, 0.0f, MoveSpeed);
	}
	if (FInputManager::GetInst().GetKey('D') == EKeyState::Held)
	{
		CameraPosition += RotationMatrix * FVector(MoveSpeed, 0.0f, 0.0f);
	}

	ViewMatrix = CreateLookAt();

	// ���콺 ��Ÿ �ʱ�ȭ
	FInputManager::GetInst().ResetMouseDeltas();
}

void UCameraComponent::SetPosition(float x, float y, float z)
{
	CameraPosition.X = x;
	CameraPosition.Y = y;
	CameraPosition.Z = z;
}

void UCameraComponent::SetRotation(float x, float y, float z)
{
	CameraRotation.X = x;
	CameraRotation.Y = y;
	CameraRotation.Z = z;
}

void UCameraComponent::SetViewportSize(float Width, float Height)
{
	// ����Ʈ ����
	ViewportWidth = Width;
	ViewportHeight = Height;
}

FVector UCameraComponent::GetPosition()
{
	return CameraPosition;
}

FVector UCameraComponent::GetRotation()
{
	return CameraRotation;
}


void UCameraComponent::GetViewMatrix(FMatrix& InViewMatrix)
{
	InViewMatrix = ViewMatrix;
	return;
}

void UCameraComponent::UpdateRotationFromMouse()
{
	// ���콺 ��Ŭ���� ������ �ʾҴٸ� ȸ������ ���� 
	if (FInputManager::GetInst().GetKey(VK_RBUTTON) != EKeyState::Held)
	{
		return;
	}

	// ���콺 �̵��� ��������
	float DeltaX = FInputManager::GetInst().GetMouseDeltaX();
	float DeltaY = FInputManager::GetInst().GetMouseDeltaY();


	// ���� ����
	float Sensitivity = 0.002f;

	// ȸ�� �� ������Ʈ
	CameraRotation.Y += DeltaX * Sensitivity; // Yaw (�¿� ȸ��)
	CameraRotation.X += DeltaY * Sensitivity; // Pitch (���� ȸ��)

	// Pitch ȸ�� ���� (���� �ʹ� ���� ���̴� �� ����)
	float MaxPitch = DirectX::XMConvertToRadians(89.0f);
	float MinPitch = DirectX::XMConvertToRadians(-89.0f);
	CameraRotation.X = (CameraRotation.X < MinPitch) ? MinPitch : (CameraRotation.X > MaxPitch) ? MaxPitch : CameraRotation.X;
}

FVector UCameraComponent::GetRayDirection(int ScreenX, int ScreenY)
{ 
	//��ũ�� ��ǥ�� ����ȭ�� ��ġ ��ǥ�� ��ȯ
	//float NDC_X = (2.0f * ScreenX)  /
	return FVector();
}

