#include <windows.h>

// D3D ��뿡 �ʿ��� ���̺귯������ ��ũ�մϴ�.
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// D3D ��뿡 �ʿ��� ������ϵ��� �����մϴ�.
#include <d3d11.h>
#include <d3dcompiler.h>

// ���߿� GUI �۾��� ���� �ű��
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

#include "Core/Sphere/Sphere.h"
#include "Engine/Renderer/URenderer.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ���� �޽����� ó���� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) {
        return true;
    }

    switch (message) {
    case WM_DESTROY:
        // Signal that the app should quit
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // ������ Ŭ���� �̸�
    WCHAR WindowClass[] = L"Wild Engine";

    // ������ Ÿ��Ʋ�ٿ� ǥ�õ� �̸�
    WCHAR Title[] = L"Wild Engine";

    // ���� �޽����� ó���� �Լ��� WndProc�� �Լ� �����͸� WindowClass ����ü�� �ִ´�.
    WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

    // ������ Ŭ���� ���
    RegisterClassW(&wndclass);

    // 1024 x 1024 ũ�⿡ ������ ����
    HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
        nullptr, nullptr, hInstance, nullptr);

    // Renderer Class�� �����մϴ�.
    URenderer* MainRender = new URenderer;

    // D3D11 �����ϴ� �Լ��� ȣ���մϴ�.
    MainRender->Create(hWnd);
    // ������ ���� ���Ŀ� ���̴��� �����ϴ� �Լ��� ȣ���մϴ�.
    MainRender->CreateShader();
    MainRender->CreateConstantBuffer();

    bool bIsExit = false;

    // FPS ������ ���� ����
    const int targetFPS = 60;
    const double targetFrameTime = 1000.0 / targetFPS; // �� �������� ��ǥ �ð� (�и��� ����)

    // ���� Ÿ�̸� �ʱ�ȭ
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER startTime, endTime;
    double elapsedTime = 0.0;
    double deltaTime = 0.0;

    // Main Loop (Quit Message�� ������ ������ �Ʒ� Loop�� ������ �����ϰ� ��)
    while (bIsExit == false) {
        // ���� ���� �ð� ���
        QueryPerformanceCounter(&startTime);

        MSG msg;

        // ó���� �޽����� �� �̻� ������ ���� ����
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            // Ű �Է� �޽����� ����
            TranslateMessage(&msg);

            // �޽����� ������ ������ ���ν����� ����, �޽����� ������ ����� WndProc ���� ���޵�
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                bIsExit = true;
                break;
            }
        }

            // MainRender
            // 
            MainRender->Update(deltaTime);

            do {
                Sleep(0);

                // ���� ���� �ð� ���
                QueryPerformanceCounter(&endTime);

                // �� �������� �ҿ�� �ð� ��� (�и��� ������ ��ȯ)
                elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

            } while (elapsedTime < targetFrameTime);

            deltaTime = elapsedTime / 1000.0;
        }

        // ReleaseShader() ������ �Ҹ� �Լ��� �߰��մϴ�.
        MainRender->ReleaseConstantBuffer();
        MainRender->ReleaseShader();
        MainRender->Release();

        return 0;
}