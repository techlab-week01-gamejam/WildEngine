#include "URenderer.h"

#include "Scene/Scene.h"
#include "Types/CommonTypes.h"

// ������ �ʱ�ȭ: ��ġ, ���� ü��, ������ ����, �����Ͷ����� ���� ����
void URenderer::Create(HWND hWindow)
{
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateRasterizerState();
    // (���� ���ٽ� ���� �� ���� ���� �� �߰� ���� ������ �ʿ信 ���� ����)
    CreateStencilState();

    CreateShader();
    CreateMatrixBuffer();
}

// Direct3D ��ġ �� ���� ü�� ����
void URenderer::CreateDeviceAndSwapChain(HWND hWindow)
{
    // ������ Direct3D ��� ���� ����
    D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // ���� ü�� ���� ����ü �ʱ�ȭ
    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Width = 0;      // â ũ�⿡ �°� �ڵ� ����
    swapchaindesc.BufferDesc.Height = 0;     // â ũ�⿡ �°� �ڵ� ����
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchaindesc.SampleDesc.Count = 1;      // ��Ƽ ���ø� ��Ȱ��ȭ
    swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchaindesc.BufferCount = 2;           // ���� ���۸�
    swapchaindesc.OutputWindow = hWindow;
    swapchaindesc.Windowed = TRUE;
    swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    // Direct3D ��ġ�� ���� ü�� ����
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
        featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
        &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

    // ���� ü�� ���� ����ȸ �� ����Ʈ ����
    SwapChain->GetDesc(&swapchaindesc);
    ViewportInfo.TopLeftX = 0.0f;
    ViewportInfo.TopLeftY = 0.0f;
    ViewportInfo.Width = static_cast<float>(swapchaindesc.BufferDesc.Width);
    ViewportInfo.Height = static_cast<float>(swapchaindesc.BufferDesc.Height);
    ViewportInfo.MinDepth = 0.0f;
    ViewportInfo.MaxDepth = 1.0f;
}

// Direct3D ��ġ �� ���� ü�� ����
void URenderer::ReleaseDeviceAndSwapChain()
{
    if (DeviceContext)
    {
        DeviceContext->Flush();
    }
    if (SwapChain)
    {
        SwapChain->Release();
        SwapChain = nullptr;
    }
    if (Device)
    {
        Device->Release();
        Device = nullptr;
    }
    if (DeviceContext)
    {
        DeviceContext->Release();
        DeviceContext = nullptr;
    }
}

// ������ ����(�� ���� �ؽ�ó) ���� �� ���� Ÿ�� �� ����
void URenderer::CreateFrameBuffer()
{
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);

    CreateStencilBuffer();
}

// ������ ���� ����
void URenderer::ReleaseFrameBuffer()
{
    if (FrameBuffer)
    {
        FrameBuffer->Release();
        FrameBuffer = nullptr;
    }
    if (FrameBufferRTV)
    {
        FrameBufferRTV->Release();
        FrameBufferRTV = nullptr;
    }
}

// �����Ͷ����� ���� ���� (ä��� ���, �ø� ���� ��)
void URenderer::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerdesc = {};
    rasterizerdesc.FillMode = D3D11_FILL_SOLID;
    rasterizerdesc.CullMode = D3D11_CULL_BACK;

    Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}

// �����Ͷ����� ���� ����
void URenderer::ReleaseRasterizerState()
{
    if (RasterizerState)
    {
        RasterizerState->Release();
        RasterizerState = nullptr;
    }
}

void URenderer::CreateStencilState()
{
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // ���ٽ� �׽�Ʈ Ȱ��ȭ
    dsDesc.StencilEnable = TRUE;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // ���� (Front) �� �ĸ� (Back) ��� �����ϰ� ����
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // �׻� ���
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; // �ȼ��� �׷��� �� ���ٽ� ���� ��ü 
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace = dsDesc.FrontFace;

    Device->CreateDepthStencilState(&dsDesc, &DepthStencilState);

    // ����� ������ ��, ���ٽ� ���� 1�� �ȼ��� ���������� �ʵ��� ���� (��, ť�� ���� Ŭ����)
    D3D11_DEPTH_STENCIL_DESC dsDescGizmo = {};
    dsDescGizmo.DepthEnable = TRUE;  // ����� ���� �׽�Ʈ�� �ʿ��ϴٸ� TRUE, �ʿ������ FALSE
    dsDescGizmo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDescGizmo.DepthFunc = D3D11_COMPARISON_LESS;

    // ���ٽ� �׽�Ʈ Ȱ��ȭ
    dsDescGizmo.StencilEnable = TRUE;
    dsDescGizmo.StencilReadMask = 0xFF;
    dsDescGizmo.StencilWriteMask = 0x00; // ������ ���ٽ� ���� �������� ����

    // ����� �ȼ��� ���ٽ� ���� 1�� �������� ���������� ����
    dsDescGizmo.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    dsDescGizmo.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDescGizmo.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescGizmo.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescGizmo.BackFace = dsDescGizmo.FrontFace;

    Device->CreateDepthStencilState(&dsDescGizmo, &GizmoDepthStencilState);
}

void URenderer::CreateStencilBuffer()
{
    // ����-���ٽ� �ؽ�ó ���� ����ü
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = static_cast<UINT>(ViewportInfo.Width);
    depthStencilDesc.Height = static_cast<UINT>(ViewportInfo.Height);
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24��Ʈ ���� + 8��Ʈ ���ٽ�
    depthStencilDesc.SampleDesc.Count = 1; // ��Ƽ���ø� ��Ȱ��ȭ
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthStencilBuffer = nullptr;
    Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);

    // ����-���ٽ� �� ����
    Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &DepthStencilView);

    // �ؽ�ó�� �� ���� �� �ٷ� ���� ����
    depthStencilBuffer->Release();
}

void URenderer::ReleaseStencilState()
{
    if (DepthStencilState)
    {
        DepthStencilState->Release();
        DepthStencilState = nullptr;
    }

    if (GizmoDepthStencilState)
    {
        GizmoDepthStencilState->Release();
        GizmoDepthStencilState = nullptr;
    }
}

void URenderer::ReleaseStencilBuffer()
{
    if (DepthStencilView)
    {
        DepthStencilView->Release();
        DepthStencilView = nullptr;
    }
}

void URenderer::Update(float deltaTime)
{

    Prepare();
    PrepareShader();


    // Loop Code
    //
    //
    if (PrimaryScene)
    {
        PrimaryScene->Render();
    }

    SwapBuffer();
}

// ������ ���� �� ��� ���ҽ� ����
void URenderer::Release()
{
    ReleaseMatrixBuffer();
    ReleaseShader();
    ReleaseRasterizerState();
    ReleaseStencilBuffer();

    if (DeviceContext)
    {
        DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    }

    ReleaseFrameBuffer();
    ReleaseDeviceAndSwapChain();
}

// �� ���ۿ� ����Ʈ ���� ��ü (ȭ�� ���)
void URenderer::SwapBuffer()
{
    SwapChain->Present(1, 0); // VSync Ȱ��ȭ
}

// ���̴� ���� �� �Է� ���̾ƿ� ����
void URenderer::CreateShader()
{
    ID3DBlob* vertexshaderCSO = nullptr;
    ID3DBlob* pixelshaderCSO = nullptr;

    // ���ؽ� ���̴� ������ �� ����
    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
    Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

    // �ȼ� ���̴� ������ �� ����
    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
    Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

    // �Է� ���̾ƿ� ����
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Device->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexshaderCSO->GetBufferPointer(),
        vertexshaderCSO->GetBufferSize(),
        &SimpleInputLayout);

    // ���ؽ� ����ü�� ũ�⸦ �������� ��Ʈ���̵� ���� (FVertexSimple ���� �ʿ�)
    Stride = sizeof(FVertexType);

    vertexshaderCSO->Release();
    pixelshaderCSO->Release();
}

// ���̴� ����
void URenderer::ReleaseShader()
{
    if (SimpleInputLayout)
    {
        SimpleInputLayout->Release();
        SimpleInputLayout = nullptr;
    }
    if (SimplePixelShader)
    {
        SimplePixelShader->Release();
        SimplePixelShader = nullptr;
    }
    if (SimpleVertexShader)
    {
        SimpleVertexShader->Release();
        SimpleVertexShader = nullptr;
    }
}

// ������ �� �غ�: ���� Ÿ�� Ŭ����, ��������, ����Ʈ, ���� ����
void URenderer::Prepare()
{
    DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);
    DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // ���ٽ� �ʱ�ȭ
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DeviceContext->RSSetViewports(1, &ViewportInfo);
    DeviceContext->RSSetState(RasterizerState);
    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, DepthStencilView); // ����-���ٽ� �� �߰�
    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
}

// ���̴� �� ��� ���� �� ������ ���� ����
void URenderer::PrepareShader()
{
    DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
    DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
    DeviceContext->IASetInputLayout(SimpleInputLayout);

    if (MatrixBuffer)
    {
        DeviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer);
    }
}

// ���� ���۸� ���ε��ϰ� ���� ������ �׸��� ���� �Լ�
void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
{
    UINT offset = 0;
    DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);
    DeviceContext->Draw(numVertices, 0);
}

void URenderer::RenderGizmo(ID3D11Buffer* pBuffer, UINT numVertices)
{
    DeviceContext->OMSetDepthStencilState(GizmoDepthStencilState, 1);
    RenderPrimitive(pBuffer, numVertices);
    DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
}

// ���� ���� ����
ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexType* vertices, UINT byteWidth)
{
    D3D11_BUFFER_DESC vertexbufferdesc = {};
    vertexbufferdesc.ByteWidth = byteWidth;
    vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // ������Ʈ���� ����
    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

    ID3D11Buffer* vertexBuffer = nullptr;
    Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

    return vertexBuffer;
}

// ���� ���� ����
void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
    if (vertexBuffer)
    {
        vertexBuffer->Release();
    }
}

// ��� ���� ����
void URenderer::CreateMatrixBuffer()
{
    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = (sizeof(FMatrix) + 0xf) & 0xfffffff0; // 16����Ʈ ��� ����
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;   // �� ������ CPU�� ���� ������Ʈ
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Device->CreateBuffer(&constantbufferdesc, nullptr, &MatrixBuffer);
}

// ��� ���� ����
void URenderer::ReleaseMatrixBuffer()
{
    if (MatrixBuffer)
    {
        MatrixBuffer->Release();
        MatrixBuffer = nullptr;
    }
}

// ��� ���� ������Ʈ (���̴��� ������ �����°� ������)
void URenderer::UpdateShaderParameters(FMatrix World, FMatrix View, FMatrix Projection)
{
    if (MatrixBuffer)
    {
        D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

        DeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
        FMatrixType* constants = (FMatrixType*)constantbufferMSR.pData;
        {
            constants->MVP = World * View * Projection;
        }
        DeviceContext->Unmap(MatrixBuffer, 0);
    }
}

void URenderer::SetPrimaryScene(UScene* NewScene)
{
    if (PrimaryScene)
    {
        delete PrimaryScene;
        PrimaryScene = nullptr;
    }

    PrimaryScene = NewScene;
}