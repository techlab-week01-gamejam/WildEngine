#include "CameraComponent.h"

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
	FieldOfView = 60.0f;

	NearZ = 0.1f;
	FarZ = 1000.0f;

	CameraPosition.X = 10.0f;
	CameraPosition.Y = 20.0f;
	CameraPosition.Z = -1.0f;

	CameraRotation.X = 0.0;
	CameraRotation.Y = 0.0;
	CameraRotation.Z = 0.0;
}

FMatrix UCameraComponent::CreateLookAt()
{
	// 1. ī�޶� ȸ�� ��� ���� (�Է� ������ ���� ����)
	FMatrix RotationMatrix = FMatrix::CreateRotationRollPitchYaw(CameraRotation.X, CameraRotation.Y, CameraRotation.Z);

	// 2. ȸ�� ����� ����� �⺻ forward�� up ���� ��ȯ
	//    (LH ��ǥ�迡�� �⺻ forward = (0, 0, 1), up = (0, 1, 0))
	FVector Forward = RotationMatrix * FVector(0.0f, 0.0f, 1.0f);
	FVector Up = RotationMatrix * FVector(0.0f, 1.0f, 0.0f);

	// 3. LookAt ��ǥ ���: ī�޶� ��ġ���� Forward �������� �� ���� �̵�
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
	float m[4][4] = {
		{ Right.X,         Right.Y,         Right.Z,         D0 },
		{ NewUp.X,         NewUp.Y,         NewUp.Z,         D1 },
		{ ForwardNormalized.X, ForwardNormalized.Y, ForwardNormalized.Z, D2 },
		{ 0.0f,            0.0f,            0.0f,            1.0f }
	};

	return FMatrix(m);
}

void UCameraComponent::Render()
{
	ViewMatrix = CreateLookAt();
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

