#include "URenderer.h"
#include "Scene.h"

// ������ �ʱ�ȭ: ��ġ, ���� ü��, ������ ����, �����Ͷ����� ���� ����
void URenderer::Create(HWND hWindow)
{
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateRasterizerState();
    // (���� ���ٽ� ���� �� ���� ���� �� �߰� ���� ������ �ʿ信 ���� ����)
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

void URenderer::Update(float deltaTime)
{

    Prepare();
    PrepareShader();


    // Loop Code
    //
    //
    Render(MainScene);

    SwapBuffer();
}

void URenderer::Render(UScene* Scene)
{
    Scene->CalculateMVP();
    for (UPrimitiveComponent* Obj : Scene->SceneObjects) {
        UpdateConstant(Obj->GetMVPMatrix());
        Obj->Render();
    }
}

void URenderer::SetMainScene(UScene* Scene)
{
    MainScene = Scene;
}

// ������ ���� �� ��� ���ҽ� ����
void URenderer::Release()
{
    if (RasterizerState)
    {
        RasterizerState->Release();
        RasterizerState = nullptr;
    }
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
    Stride = sizeof(FVertexSimple);

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
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DeviceContext->RSSetViewports(1, &ViewportInfo);
    DeviceContext->RSSetState(RasterizerState);
    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

// ���̴� �� ��� ���� �� ������ ���� ����
void URenderer::PrepareShader()
{
    DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
    DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
    DeviceContext->IASetInputLayout(SimpleInputLayout);

    if (ConstantBuffer)
    {
        DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
    }
}

// ���� ���۸� ���ε��ϰ� ���� ������ �׸��� ���� �Լ�
void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
{
    UINT offset = 0;
    DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);
    DeviceContext->Draw(numVertices, 0);
}

// ���� ���� ����
ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
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
void URenderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = (sizeof(FConstants) + 0xf) & 0xfffffff0; // 16����Ʈ ��� ����
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;   // �� ������ CPU�� ���� ������Ʈ
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}

// ��� ���� ����
void URenderer::ReleaseConstantBuffer()
{
    if (ConstantBuffer)
    {
        ConstantBuffer->Release();
        ConstantBuffer = nullptr;
    }
}

// ��� ���� ������Ʈ (���̴��� ������ �����°� ������)
void URenderer::UpdateConstant(FMatrix MVP)
{
    if (ConstantBuffer)
    {
        D3D11_MAPPED_SUBRESOURCE constantbufferMSR;
        DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);
        FConstants* constants = reinterpret_cast<FConstants*>(constantbufferMSR.pData);
        constants->MVP = MVP;
        DeviceContext->Unmap(ConstantBuffer, 0);
    }
}