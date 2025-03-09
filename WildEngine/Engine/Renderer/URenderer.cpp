#include "URenderer.h"

#include "Scene/Scene.h"
#include "Types/CommonTypes.h"
#include "../Core/json.hpp"
#include "../Core/Object/Object.h"
#include "../Engine/Components/PrimitiveComponent.h"
#include <fstream>

// ������ �ʱ�ȭ: ��ġ, ���� ü��, ������ ����, �����Ͷ����� ���� ����
void URenderer::Create(HWND hWindow)
{
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateRasterizerState();
    // (���� ���ٽ� ���� �� ���� ���� �� �߰� ���� ������ �ʿ信 ���� ����)

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

void URenderer::SaveJson()
{
    //�׽�Ʈ�� ����
    uint32 Version = 1;
    uint32 NextUUID = 8;
    TArray<UObject*> GUObjectArray;
    FString SceneName = "TestScene";

    // �׽�Ʈ ��ü ����: �� ���� UPrimitiveComponent ����
    UPrimitiveComponent* Sphere = new UPrimitiveComponent();
    Sphere->UUID = 0;
    Sphere->RelativeLocation = { 0.29f, 1.21f, 0.0f };
    Sphere->RelativeRotation = { 0.f, 0.f, 0.f };
    Sphere->RelativeScale3D = { 1.f, 1.f, 1.f };
    GUObjectArray.push_back(Sphere);

    UPrimitiveComponent* Cube = new UPrimitiveComponent();
    Cube->UUID = 1;
    Cube->RelativeLocation = { -0.17f, 0.06f, 0.0f };
    Cube->RelativeRotation = { 0.f, 0.f, 0.f };
    Cube->RelativeScale3D = { 1.f, 1.f, 1.f };
    GUObjectArray.push_back(Cube);

    // Primitives�� ��ü�� �ʱ�ȭ�մϴ�.
    json::JSON Scene;
    Scene["Version"] = Version;
    Scene["NextUUID"] = NextUUID;

    // GUObjectArray�� �� ��ü�� ���� JSON �����͸� �߰�
    for (uint32 i = 0; i < GUObjectArray.size(); i++)
    {
        if (UPrimitiveComponent* Primitive = dynamic_cast<UPrimitiveComponent*>(GUObjectArray[i]))
        {
            // UUID �Ǵ� ���ϴ� �ε����� ���ڿ� Ű�� ���
            std::string key = std::to_string(Primitive->UUID);
            Scene["Primitives"][key]["Location"] = json::FVectorToJSON(Primitive->RelativeLocation);
            Scene["Primitives"][key]["Rotation"] = json::FVectorToJSON(Primitive->RelativeRotation);
            Scene["Primitives"][key]["Scale"] = json::FVectorToJSON(Primitive->RelativeScale3D);
            Scene["Primitives"][key]["Type"] = "Sphere";
        }
    }

    std::string jsonData = Scene.dump();
    std::ofstream outFile(SceneName + ".Scene");
	if (outFile.is_open())
	{
		outFile << jsonData;
		outFile.close();
        std::cout << "JSON �����͸� output.json�� �����߽��ϴ�." << std::endl;
    }
    else {
        std::cerr << "���� ���⿡ �����߽��ϴ�." << std::endl;
    }
}

void URenderer::LoadJson()
{
    std::ifstream inFile("TestScene.Scene");
    if (inFile.is_open())
    {
        std::string jsonData;
        inFile.seekg(0, std::ios::end);
        jsonData.reserve(inFile.tellg());
        inFile.seekg(0, std::ios::beg);
        jsonData.assign((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        json::JSON Scene = json::JSON::Load(jsonData);
        uint32 Version = Scene["Version"].ToInt();
        uint32 NextUUID = Scene["NextUUID"].ToInt();
        json::JSON Primitives = Scene["Primitives"];
        for (auto it = Primitives.ObjectRange().begin(); it != Primitives.ObjectRange().end(); ++it)
        {
            uint32 UUID = std::stoi(it->first);
            json::JSON Primitive = it->second;
            std::string Type = Primitive["Type"].ToString();
            FVector Location = json::JSONToFVector(Primitive["Location"]);
            FVector Rotation = json::JSONToFVector(Primitive["Rotation"]);
            FVector Scale = json::JSONToFVector(Primitive["Scale"]);
            if (Type == "Sphere")
            {
                UPrimitiveComponent* Sphere = new UPrimitiveComponent();
                Sphere->UUID = UUID;
                Sphere->RelativeLocation = Location;
                Sphere->RelativeRotation = Rotation;
                Sphere->RelativeScale3D = Scale;
            }
        }
        std::cout << "JSON �����͸� ���������� �ҷ��Խ��ϴ�." << std::endl;

		//std::cout << "Version: " + std::to_string(Version) << std::endl;
		//std::cout << "NextUUID: " + std::to_string(NextUUID) << std::endl;

  //      // Primitive Parsing: Primitives �� �� ������Ƽ�� ������ ����մϴ�.
  //      for (auto it = Primitives.ObjectRange().begin(); it != Primitives.ObjectRange().end(); ++it)
  //      {
  //          uint32 UUID = std::stoi(it->first);
  //          json::JSON Primitive = it->second;
  //          std::string Type = Primitive["Type"].ToString();
  //          FVector Location = json::JSONToFVector(Primitive["Location"]);
  //          FVector Rotation = json::JSONToFVector(Primitive["Rotation"]);
  //          FVector Scale = json::JSONToFVector(Primitive["Scale"]);

  //          // �����: ������Ƽ�� ������ ��� (OutputDebugStringA ���)
  //          std::string debugMsg;
  //          debugMsg = "Primitive UUID: " + std::to_string(UUID) + "\n";
  //          debugMsg += "  Type: " + Type + "\n";
  //          debugMsg += "  Location: (" + std::to_string(Location.X) + ", "
  //              + std::to_string(Location.Y) + ", "
  //              + std::to_string(Location.Z) + ")\n";
  //          debugMsg += "  Rotation: (" + std::to_string(Rotation.X) + ", "
  //              + std::to_string(Rotation.Y) + ", "
  //              + std::to_string(Rotation.Z) + ")\n";
  //          debugMsg += "  Scale: (" + std::to_string(Scale.X) + ", "
  //              + std::to_string(Scale.Y) + ", "
  //              + std::to_string(Scale.Z) + ")\n";
  //          std::cout << debugMsg << std::endl;
  //      }

    }
    else {
        std::cerr << "���� ���⿡ �����߽��ϴ�." << std::endl;
    }
}

