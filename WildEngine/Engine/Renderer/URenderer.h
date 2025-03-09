#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Components/PrimitiveComponent.h"

class UScene;

struct FVertexSimple
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

// ������ Ŭ���� ����
class URenderer
{
public:
    // Direct3D ��ġ �� ���� ��ü ������
    ID3D11Device* Device = nullptr;                   // Direct3D ��ġ
    ID3D11DeviceContext* DeviceContext = nullptr;       // ��ġ ���ؽ�Ʈ
    IDXGISwapChain* SwapChain = nullptr;                // ���� ü��

    // ������ ���ҽ� �� ����
    ID3D11Texture2D* FrameBuffer = nullptr;             // �� ���� �ؽ�ó
    ID3D11RenderTargetView* FrameBufferRTV = nullptr;     // ���� Ÿ�� ��
    ID3D11RasterizerState* RasterizerState = nullptr;     // �����Ͷ����� ����
    ID3D11Buffer* ConstantBuffer = nullptr;             // ��� ����

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };  // �ʱ� Ŭ���� ���� (RGBA)
    D3D11_VIEWPORT ViewportInfo;                        // ����Ʈ ����

    // ���̴� ���� ���
    ID3D11VertexShader* SimpleVertexShader = nullptr;
    ID3D11PixelShader* SimplePixelShader = nullptr;
    ID3D11InputLayout* SimpleInputLayout = nullptr;
    unsigned int Stride = 0;

    TArray<UPrimitiveComponent*> SceneObjects;

public:
    // ���� �� ���� ���� �Լ�
    void Create(HWND hWindow);
    void Release();

    // ������ ó�� �Լ�
    void SwapBuffer();
    void Prepare();
    void PrepareShader();
    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);
    ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);
    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);

    // ��� ���� ���� �Լ�
    void CreateConstantBuffer();
    void ReleaseConstantBuffer();
    void UpdateConstant(FMatrix MVP);


    // ���̴� ����/���� �Լ�
    void CreateShader();
    void ReleaseShader();

    // ���������� ���Ǵ� Direct3D ��ġ �� ���� ü�� ���� �Լ�
    void CreateDeviceAndSwapChain(HWND hWindow);
    void ReleaseDeviceAndSwapChain();

    // ������ ���� �� �����Ͷ����� ���� ���� �Լ�
    void CreateFrameBuffer();
    void ReleaseFrameBuffer();
    void CreateRasterizerState();
    void ReleaseRasterizerState();

    void Update(float deltaTime);
    void Render(UScene* Scene);

    void SetMainScene(UScene* Scene);

private:
    // ��� ���ۿ� ���� ����ü
    struct FConstants
    {
        FMatrix MVP;
    };

    UScene* MainScene;
};
