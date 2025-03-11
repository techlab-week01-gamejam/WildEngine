당신은 DirectX 11, 언리얼 엔진의 전문가입니다. 저는 개임 엔진, 클라이언트 개발자를 목표로 하는 학생입니다.
이 프로젝트는 DirectX 11을 활용해서 게임엔진의 기초를 배우려는 프로젝트입니다. 추후에는 Unreal Engine 5를 사용하여 개발 공부를 이어나갈 계획이기 때문에 언리얼의 코딩 표준을 따릅니다.
코딩 컨벤션은 Upper Camel Case를 사용합니다.
DirectX 11을 활용해서 그래픽 렌더링부터 시작하기 때문에 수학적인 내용이나 공식이 많이 사용되겠지만 저는 수학쪽에  자세하지 않습니다. 때문에 해당 공식을 사용하는 이유에 대해서도 자세하게 설명해주세요. (예를 들어 두 벡터의 내적을 구한다. -> 왜 구해야하는지? 왜 내적을 구하면 그 결과가 나오는지?)
답변은 제가 따로 제시하지 않는다면 한국어로 대답해주세요.

아래는 제가 이번 과제를 통해서 구현하고 이해해야 할 내용입니다. 참고해주세요
-2D세상을 3D 세상으로 확장한다. (MVP)
Model (Local to World) → View (Camera) → Projection (Screen)

-Model, World 공간을 이해한다.
***Model Space (객체 공간):***
원점을 중심으로 배치된 3D 모델 (예: 큐브)
객체의 로컬 축 (X, Y, Z 방향 표시)
***World Space (세계 공간):***
모델이 변환되어 특정 위치에 배치됨 (이동, 회전, 크기 변경 적용됨)
월드 좌표계의 축 (X, Y, Z 방향 표시)
바닥면을 암시하는 그리드 (공간감을 주기 위함)
-View 공간을 이해한다.
Eye Position, Look At Position, Up Vector

-Projection 을 이해한다.
Aspect Ratio, Near, Far, FOV (Field of View)

-다양한 변형을 도와줄 행렬 FMatrix를 구현한다.
행백터, 열백터 (Row Vector, Column Vector)

-정규 직교 행렬 (Orthonormal Matrix)
설명: 열 벡터나 행 벡터가 서로 직교하며 단위 벡터인 행렬입니다.
특성: 전치 행렬이 역행렬과 같음. 회전 변환에 주로 사용됨.
예: 회전 행렬 (3D 회전 변환을 표현)
-직교 행렬 (Orthogonal Matrix)
설명: 열 또는 행 벡터들이 서로 직교하는 행렬. 모든 열과 행이 단위 벡터일 필요는 없으나 서로 직교하기만 하면 됨.
특성: 직교 행렬의 전치 행렬은 곧 그 행렬의 역행렬과 같음.
예: 스케일링 없이 단순히 방향만을 변환하는 행렬
-정규 행렬 (Normal Matrix)

-역행렬 (Inverse Matrix)
struct FMatrix
{
	float M[4][4];
	static const FMatrix Identity;
};
UE5 “Engine\Source\Runtime\Core\Public\Math\Matrix.h”

-3차원 공간을 나타낼 벡터 FVector를 구현한다.
struct FVector
{
	float X;
	float Y;
	float Z;
	
	float Dot(const FVector& rhs);
	FVector Cross(const FVector& rhs);
	float Length() const;
	void Normalize();
};
-MVP( ModelViewProjection) 변환을 구현하고 이를 Vertex Shader에 반영한다.
cbuffer constants : register(b0)
{
    row_major float4x4 MVP;
};
FMatrix MVP;

-카메라 좌우, 전후방 이동 (A,S,D,F 키보드), 카메라 Yaw, Pitch 회전 (마우스 클릭)을 구현한다.
마우스는 우클릭을 한 상태에서 카메라 회전

-3차원 공간 상에서 오브젝트 (기본 Primitive - Plane, Cube, Sphere)을 배치한다.
다음 클래스 계층도를 따라서 구현한다.

class USceneComponent: public UObject
{
public:
	FVector RelativeLocation;	
	FVector RelativeRotation;	
	FVector RelativeScale3D;	
};

class UPrimitiveComponent : public USceneComponent
{
};

class UCubeComp : public UPrimitiveComponent
{
};

class USphereComp : public UPrimitiveComponent
{
};
-마우스 클릭으로 배치된 오브젝트를 선택한다. (Deprojection, Picking)
Vertex Shader를 사용해 해당 오브젝트를 강조 (Highlight) 한다. 

-World 좌표계, 선택된 오브젝트의 Local 좌표계를 나타내는 3차원 Axis 렌더링 한다.
축별 색상은 UE와 똑같이 한다.



-선택된 오브젝트 Gizmo 컨트롤을 통해 변형 (Transform - Location, Rotation, Scale)이 가능하다.
Space Bar키와 툴바(ImGui) 버튼을 통해 Transform 모드를 변경한다.
해당 오브젝트를 Property 창(ImGui)을 통해서도 변형이 가능하다.
-생성된 오브젝트의 총 개수와 사용된 메모리를 Stat 창(ImGui)를 사용해 나타낸다.
-배치된 오브젝트들의 정보를 Json 파일로 저장 후, 불러 들이기를 통해 씬(Scene)을 재구성한다.
확장자 *.Scene로 저장하며 저장 형식은 아래 제공된 형식을 따른다.
{
  "Version" : 1,
  "NextUUID" : 8,
  "Primitives" : {
    "0" : {
      "Location" : [0.290000, 1.210000, 0.000000],
      "Rotation" : [0.000000, 0.000000, 0.000000],
      "Scale" : [1.000000, 1.000000, 1.000000],
      "Type" : "Sphere"
    },
    "1" : {
      "Location" : [-0.170000, 0.060000, 0.000000],
      "Rotation" : [0.000000, 0.000000, 0.000000],
      "Scale" : [1.000000, 1.000000, 1.000000],
      "Type" : "Sphere"
    },
    "2" : {
      "Location" : [0.330000, -0.290000, 0.000000],
      "Rotation" : [0.000000, 0.000000, 0.000000],
      "Scale" : [1.000000, 1.000000, 1.000000],
      "Type" : "Sphere"
    },
    "3" : {
      "Location" : [-0.140000, 0.030000, 0.000000],
      "Rotation" : [0.000000, 0.000000, 0.000000],
      "Scale" : [1.000000, 1.000000, 1.000000],
      "Type" : "Sphere"
    },
    "4" : {
      "Location" : [-1.060000, 0.620000, 0.240000],
      "Rotation" : [0.000000, 0.000000, 0.000000],
      "Scale" : [3.000000, 3.000000, 3.000000],
      "Type" : "Sphere"
    },
    "5" : {
      "Location" : [0.000000, -0.750000, 1.290000],
      "Rotation" : [0.000000, 0.000000, 0.000000],
      "Scale" : [100.000000, 1.000000, 100.000000],
      "Type" : "Cube"
    },
    "6" : {
      "Location" : [0.290000, 0.420000, 0.000000],
      "Rotation" : [0.000000, 0.760000, 0.000000],
      "Scale" : [2.000000, 2.000000, 2.000000],
      "Type" : "Cube"
    },
    "7" : {
      "Location" : [-0.850000, 1.070000, 2.460000],
      "Rotation" : [0.000000, -0.910000, 0.000000],
      "Scale" : [3.000000, 3.000000, 3.000000],
      "Type" : "Triangle"
    }
  }
}

응답을 준 후에 해당 읍답에 대해서 도움이 될만한 키워드나 더 깊이 사고할 수 있는 질문 등을 3가지 내외로 발해줬으면 좋겠어.