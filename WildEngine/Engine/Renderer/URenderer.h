#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Math/Matrix.h"

struct FVertexType;

class UScene;
class UWildEditor;

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
    ID3D11DepthStencilState* DepthStencilState = nullptr; // ���Ľ� ����
    ID3D11DepthStencilState* GizmoDepthStencilState = nullptr; // Gizmo ���Ľ� ����
    ID3D11Buffer* MatrixBuffer = nullptr;             // ��� ����

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };  // �ʱ� Ŭ���� ���� (RGBA)
    D3D11_VIEWPORT ViewportInfo;                        // ����Ʈ ����

    // ���̴� ���� ���
    ID3D11VertexShader* SimpleVertexShader = nullptr;
    ID3D11PixelShader* SimplePixelShader = nullptr;
    ID3D11InputLayout* SimpleInputLayout = nullptr;
    unsigned int Stride = 0;


    ID3D11DepthStencilView* DepthStencilView = nullptr;

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

    // �׸���
    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);

    void RenderGizmo(ID3D11Buffer* pBuffer, UINT numVertices);

    void SetPrimaryScene(UScene* NewScene);

    UScene* GetPrimaryScene() { return PrimaryScene; }

    UWildEditor* GetPrimaryEditor() { return PrimaryEditor; }

    void OnResize(uint32 Width, uint32 Height);
private:
    // ������ ó�� �Լ�
    void SwapBuffer();
    void Prepare();
    void PrepareShader();

    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);

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
    
    // �����Ͷ����� ����
    void CreateRasterizerState();
    void ReleaseRasterizerState();

    // ���ٽ� ����
    void CreateStencilState();
    void CreateStencilBuffer();
    void ReleaseStencilState();
    void ReleaseStencilBuffer();

private:
    UScene* PrimaryScene;

    UWildEditor* PrimaryEditor;

    struct FMatrixType
    {
        FMatrix MVP;
    };

};