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
	// 1. 카메라 회전 행렬 생성 (입력 각도는 라디안 단위)
	FMatrix RotationMatrix = FMatrix::CreateRotationRollPitchYaw(CameraRotation.X, CameraRotation.Y, CameraRotation.Z);

	// 2. 회전 행렬을 사용해 기본 forward와 up 벡터 변환
	//    (LH 좌표계에서 기본 forward = (0, 0, 1), up = (0, 1, 0))
	FVector Forward = RotationMatrix * FVector(0.0f, 0.0f, 1.0f);
	FVector Up = RotationMatrix * FVector(0.0f, 1.0f, 0.0f);

	// 3. LookAt 좌표 계산: 카메라 위치에서 Forward 방향으로 한 단위 이동
	FVector LookAt = CameraPosition + Forward;

	// 4. 정규직교 좌표계 구성
	//    ForwardNormalized는 카메라가 바라보는 실제 방향
	FVector ForwardNormalized = (LookAt - CameraPosition).Normalized();
	// LH 좌표계: Right = normalize(cross(Up, ForwardNormalized))
	FVector Right = Up.Cross(ForwardNormalized).Normalized();
	// 보정된 Up 벡터: NewUp = cross(ForwardNormalized, Right)
	FVector NewUp = ForwardNormalized.Cross(Right);

	// 5. 평행 이동 요소 계산: 각 축 벡터와 카메라 위치의 내적의 음수
	float D0 = -Right.Dot(CameraPosition);
	float D1 = -NewUp.Dot(CameraPosition);
	float D2 = -ForwardNormalized.Dot(CameraPosition);

	// 6. 뷰 행렬 구성 (row-major 방식)
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

