#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Math/Matrix.h"

struct FVertexType;
class UScene;

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
    ID3D11Buffer* MatrixBuffer = nullptr;             // ��� ����

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };  // �ʱ� Ŭ���� ���� (RGBA)
    D3D11_VIEWPORT ViewportInfo;                        // ����Ʈ ����

    // ���̴� ���� ���
    ID3D11VertexShader* SimpleVertexShader = nullptr;
    ID3D11PixelShader* SimplePixelShader = nullptr;
    ID3D11InputLayout* SimpleInputLayout = nullptr;
    unsigned int Stride = 0;


public:
    // ���� �� ���� ���� �Լ�
    void Create(HWND hWindow);
    void Release();

    // ������Ʈ �Լ�
    void Update(float deltaTime);

    // ��� ���� ������Ʈ
    void UpdateShaderParameters(FMatrix World, FMatrix View, FMatrix Projection);

    // ��� ���� ����
    ID3D11Buffer* CreateVertexBuffer(FVertexType* vertices, UINT byteWidth);

    ID3D11Buffer* CreateIndexBuffer(const TArray<uint16_t>& indices);

    // �׸���
    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);

    void RenderGizmo(ID3D11Buffer* VertexBuffer, ID3D11Buffer* IndexBuffer, int NumVertices, int NumIndices);

    void SetPrimaryScene(UScene* NewScene);

private:
    // ������ ó�� �Լ�
    void SwapBuffer();
    void Prepare();
    void PrepareShader();

    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);

    void ReleaseIndexBuffer(ID3D11Buffer* indexBuffer);

    // ��� ���� ���� �Լ�
    void CreateMatrixBuffer();
    void ReleaseMatrixBuffer();

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

private:
    UScene* PrimaryScene;

    struct FMatrixType
    {
        FMatrix MVP;
    };

};