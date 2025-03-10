#include "WildEditor.h"


#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../imGui/imgui_impl_win32.h"

#include "Renderer/URenderer.h"
#include "Scene/Scene.h"

#include "EditorDesigner.h"
#include "Editor/Windows/ControlWindow.h"
#include "Editor/Windows/PropertyWindow.h"
#include "Editor/Windows/ConsoleWindow.h"
#include "Editor/Windows/StatWindow.h"

#include "Font/IconDefs.h"
#include "Font/RawFonts.h"

#include "Scene/Scene.h"
#include <Components/PrimitiveComponent.h>

UWildEditor::UWildEditor(URenderer* InRenderer)
{
    Renderer = InRenderer;
}

UWildEditor::UWildEditor(const UWildEditor&)
{
}

UWildEditor::~UWildEditor()
{
}

void UWildEditor::Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, HWND hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init((void*)hWnd);
    ImGui_ImplDX11_Init(Device, DeviceContext);

    CreateUsingFont();
    
    auto NewControlWindow = std::make_shared<ControlWindow>();
    UEditorDesigner::Get().AddWindow("ControlWindow", NewControlWindow);

    NewPropertyWindow = std::make_shared<PropertyWindow>();
    UEditorDesigner::Get().AddWindow("PropertyWindow", NewPropertyWindow);

    auto NewConsoleWindow = std::make_shared<ConsoleWindow>();
    UEditorDesigner::Get().AddWindow("ConsoleWindow", NewConsoleWindow);

    auto NewStatWindow = std::make_shared<StatWindow>();
    UEditorDesigner::Get().AddWindow("StatWindow", NewStatWindow);

}

void UWildEditor::Release()
{
    Renderer = nullptr;

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UWildEditor::Render()
{
    PreferenceStyle();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    Scene = Renderer->GetPrimaryScene();

    SetupControlWindow();
    SetupPropertyWindow();

    UEditorDesigner::Get().Render();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UWildEditor::PreferenceStyle()
{
    // Window
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0.9f);
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0.5, 0, 1.0f);
    ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
    ImGui::GetStyle().WindowRounding = 5.0f;

    ImGui::GetStyle().FrameRounding = 3.0f;

    // Sep
    ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImVec4(0.3, 0.3, 0.3, 1.0f);
    
    // Frame
    ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.31, 0.31, 0.31, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImVec4(0.203, 0.203, 0.203, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImVec4(0, 0.5, 0, 0.6f);

    // Button
    ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.105, 0.105, 0.105, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.105, 0.105, 0.105, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0.5, 0, 0.6f);

    ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(0.203, 0.203, 0.203, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImVec4(0.105, 0.105, 0.105, 0.6f);
    ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImVec4(0, 0.5, 0, 0.6f);

    // Text
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 0.9);
    
}

void UWildEditor::CreateUsingFont()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImFontConfig FeatherFontConfig;
    FeatherFontConfig.PixelSnapH = true;
    FeatherFontConfig.FontDataOwnedByAtlas = false;
    FeatherFontConfig.GlyphOffset = ImVec2(0, 0);
    static const ImWchar IconRanges[] = { 
        ICON_MOVE,      ICON_MOVE + 1, 
        ICON_ROTATE,    ICON_ROTATE + 1, 
        ICON_SCALE,     ICON_SCALE + 1, 
        ICON_MONITOR,   ICON_MONITOR + 1,
        ICON_BAR_GRAPH, ICON_BAR_GRAPH +1,
        ICON_NEW,       ICON_NEW + 1,
        ICON_SAVE,      ICON_SAVE + 1,
        ICON_LOAD,      ICON_LOAD + 1,
        0 };

    io.Fonts->AddFontFromMemoryTTF(FeatherRawData, FontSizeOfFeather, 22.0f, &FeatherFontConfig, IconRanges);
}

void UWildEditor::SetupControlWindow()
{
    auto Window = UEditorDesigner::Get().GetWindow("ControlWindow");
    if (Window)
    {
        if (ControlWindow* Control = dynamic_cast<ControlWindow*>(Window.get()))
        {
            Control->SetPrimaryGizmo(Scene->GetGizmo());
        }
    }
}

void UWildEditor::SetupPropertyWindow()
{
    auto Window = UEditorDesigner::Get().GetWindow("PropertyWindow");
    if (Window)
    {
        if (PropertyWindow* Property = dynamic_cast<PropertyWindow*>(Window.get()))
        {
            if (UPrimitiveComponent* SelectedObject = dynamic_cast<UPrimitiveComponent*>(Scene->GetSelectedObject())) {
                Property->SetLocation(SelectedObject->RelativeLocation);
                Property->SetRotation(SelectedObject->RelativeRotation);
                Property->SetScale(SelectedObject->RelativeScale3D);

                Property->SetUUID(SelectedObject->UUID);
            }
        }
    }
}