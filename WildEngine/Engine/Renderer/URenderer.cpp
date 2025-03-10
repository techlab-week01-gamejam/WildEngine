#include "URenderer.h"

#include "Scene/Scene.h"
#include "Types/CommonTypes.h"
#include "../Core/json.hpp"
#include "../Core/Object/Object.h"
#include "../Engine/Components/PrimitiveComponent.h"
#include <fstream>
#include "Editor/WildEditor.h"

#include "Input/InputManager.h"

// 렌더러 초기화: 장치, 스왑 체인, 프레임 버퍼, 래스터라이저 상태 생성
void URenderer::Create(HWND hWindow)
{
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateRasterizerState();
    // (깊이 스텐실 버퍼 및 블렌드 상태 등 추가 상태 설정은 필요에 따라 구현)
    CreateStencilState();

    CreateShader();
    CreateMatrixBuffer();

    if (PrimaryEditor == nullptr)
    {
        PrimaryEditor = new UWildEditor(this);
    }
    PrimaryEditor->Create(Device, DeviceContext, hWindow);
}

// Direct3D 장치 및 스왑 체인 생성
void URenderer::CreateDeviceAndSwapChain(HWND hWindow)
{
    // 지원할 Direct3D 기능 레벨 정의
    D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // 스왑 체인 설정 구조체 초기화
    DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
    swapchaindesc.BufferDesc.Width = 0;      // 창 크기에 맞게 자동 설정
    swapchaindesc.BufferDesc.Height = 0;     // 창 크기에 맞게 자동 설정
    swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchaindesc.SampleDesc.Count = 1;      // 멀티 샘플링 비활성화
    swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchaindesc.BufferCount = 2;           // 더블 버퍼링
    swapchaindesc.OutputWindow = hWindow;
    swapchaindesc.Windowed = TRUE;
    swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    // Direct3D 장치와 스왑 체인 생성
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
        featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
        &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

    // 스왑 체인 정보 재조회 후 뷰포트 설정
    SwapChain->GetDesc(&swapchaindesc);
    ViewportInfo.TopLeftX = 0.0f;
    ViewportInfo.TopLeftY = 0.0f;
    ViewportInfo.Width = static_cast<float>(swapchaindesc.BufferDesc.Width);
    ViewportInfo.Height = static_cast<float>(swapchaindesc.BufferDesc.Height);
    ViewportInfo.MinDepth = 0.0f;
    ViewportInfo.MaxDepth = 1.0f;
}

// Direct3D 장치 및 스왑 체인 해제
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

// 프레임 버퍼(백 버퍼 텍스처) 생성 및 렌더 타겟 뷰 생성
void URenderer::CreateFrameBuffer()
{
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);

    CreateStencilBuffer();
}

// 프레임 버퍼 해제
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

// 래스터라이저 상태 생성 (채우기 모드, 컬링 설정 등)
void URenderer::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerdesc = {};
    rasterizerdesc.FillMode = D3D11_FILL_SOLID;
    rasterizerdesc.CullMode = D3D11_CULL_BACK;

    Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}

// 래스터라이저 상태 해제
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

    // 스텐실 테스트 활성화
    dsDesc.StencilEnable = TRUE;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // 전면 (Front) 및 후면 (Back) 모두 동일하게 설정
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 항상 통과
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; // 픽셀이 그려질 때 스텐실 값을 교체 
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace = dsDesc.FrontFace;

    Device->CreateDepthStencilState(&dsDesc, &DepthStencilState);

    // 기즈모 렌더링 시, 스텐실 값이 1인 픽셀은 렌더링하지 않도록 설정 (즉, 큐브 내부 클리핑)
    D3D11_DEPTH_STENCIL_DESC dsDescGizmo = {};
    dsDescGizmo.DepthEnable = TRUE;  // 기즈모에 깊이 테스트가 필요하다면 TRUE, 필요없으면 FALSE
    dsDescGizmo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDescGizmo.DepthFunc = D3D11_COMPARISON_LESS;

    // 스텐실 테스트 활성화
    dsDescGizmo.StencilEnable = TRUE;
    dsDescGizmo.StencilReadMask = 0xFF;
    dsDescGizmo.StencilWriteMask = 0x00; // 기즈모는 스텐실 값을 변경하지 않음

    // 기즈모 픽셀은 스텐실 값이 1인 곳에서는 렌더링되지 않음
    dsDescGizmo.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    dsDescGizmo.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDescGizmo.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescGizmo.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescGizmo.BackFace = dsDescGizmo.FrontFace;

    Device->CreateDepthStencilState(&dsDescGizmo, &GizmoDepthStencilState);
}

void URenderer::CreateStencilBuffer()
{
    // 깊이-스텐실 텍스처 설명 구조체
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = static_cast<UINT>(ViewportInfo.Width);
    depthStencilDesc.Height = static_cast<UINT>(ViewportInfo.Height);
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24비트 깊이 + 8비트 스텐실
    depthStencilDesc.SampleDesc.Count = 1; // 멀티샘플링 비활성화
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthStencilBuffer = nullptr;
    Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);

    // 깊이-스텐실 뷰 생성
    Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &DepthStencilView);

    // 텍스처는 뷰 생성 후 바로 해제 가능
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
    FInputManager::GetInst().Tick(deltaTime);
    //
    if (PrimaryScene)
    {
        PrimaryScene->Render();
    }

    // Editor Render
    if (PrimaryEditor)
    {
        PrimaryEditor->Render();
    }

    SwapBuffer();
}

// 렌더러 종료 시 모든 리소스 해제
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

// 백 버퍼와 프론트 버퍼 교체 (화면 출력)
void URenderer::SwapBuffer()
{
    SwapChain->Present(1, 0); // VSync 활성화
}

// 쉐이더 생성 및 입력 레이아웃 설정
void URenderer::CreateShader()
{
    ID3DBlob* vertexshaderCSO = nullptr;
    ID3DBlob* pixelshaderCSO = nullptr;

    // 버텍스 쉐이더 컴파일 및 생성
    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
    Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

    // 픽셀 쉐이더 컴파일 및 생성
    D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
    Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

    // 입력 레이아웃 설정
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Device->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexshaderCSO->GetBufferPointer(),
        vertexshaderCSO->GetBufferSize(),
        &SimpleInputLayout);

    // 버텍스 구조체의 크기를 기준으로 스트라이드 설정 (FVertexSimple 정의 필요)
    Stride = sizeof(FVertexType);

    vertexshaderCSO->Release();
    pixelshaderCSO->Release();
}

// 쉐이더 해제
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

// 렌더링 전 준비: 렌더 타겟 클리어, 토폴로지, 뷰포트, 상태 설정
void URenderer::Prepare()
{
    DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);
    DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // 스텐실 초기화
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DeviceContext->RSSetViewports(1, &ViewportInfo);
    DeviceContext->RSSetState(RasterizerState);
    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, DepthStencilView); // 깊이-스텐실 뷰 추가
    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
}

// 쉐이더 및 상수 버퍼 등 렌더링 상태 설정
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

// 정점 버퍼를 바인딩하고 원시 도형을 그리기 위한 함수
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

// 정점 버퍼 생성
ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexType* vertices, UINT byteWidth)
{
    D3D11_BUFFER_DESC vertexbufferdesc = {};
    vertexbufferdesc.ByteWidth = byteWidth;
    vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // 업데이트되지 않음
    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

    ID3D11Buffer* vertexBuffer = nullptr;
    Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

    return vertexBuffer;
}

// 정점 버퍼 해제
void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
    if (vertexBuffer)
    {
        vertexBuffer->Release();
    }
}

// 상수 버퍼 생성
void URenderer::CreateMatrixBuffer()
{
    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = (sizeof(FMatrix) + 0xf) & 0xfffffff0; // 16바이트 배수 보장
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;   // 매 프레임 CPU에 의해 업데이트
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Device->CreateBuffer(&constantbufferdesc, nullptr, &MatrixBuffer);
}

// 상수 버퍼 해제
void URenderer::ReleaseMatrixBuffer()
{
    if (MatrixBuffer)
    {
        MatrixBuffer->Release();
        MatrixBuffer = nullptr;
    }
}

// 행렬 버퍼 업데이트 (쉐이더에 전달할 오프셋과 스케일)
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

