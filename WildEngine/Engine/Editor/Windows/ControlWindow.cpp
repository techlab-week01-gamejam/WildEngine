#include "ControlWindow.h"
#include "../../../ImGui/imgui.h"
#include "../../../ImGui/imgui_internal.h"

#include "Editor/EditorDesigner.h"

ControlWindow::ControlWindow()
{
	FramePerSecond = 0;
	
	PrimtiveTypeNumber = 0;
	SpawnNumber = 0;
	SceneName = "NewScene";
	bIsOrthogonal = false;

	FOV = 60;
	CameraLocation.Set(0,1,0);
	CameraRotation.Set(0,0,0);

    Location[0] = CameraLocation.X;
    Location[1] = CameraLocation.Y;
    Location[2] = CameraLocation.Z;


    Rotation[0] = CameraRotation.X;
    Rotation[1] = CameraRotation.Y;
    Rotation[2] = CameraRotation.Z;

    WindowWidth = 360;
    WindowHeight = 300;
}

void ControlWindow::Render()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(WindowWidth, WindowHeight), ImGuiCond_Always);

	ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoResize);

	ImGui::Text("WILD ENGINE - KRFTON JUNGLE");
	ImGui::Text("FPS %.0f (%.0f ms)", io.Framerate, 1000.0f / io.Framerate);

    ImGui::Separator(); // ���� ���м�
    // ��Ʈ�� ��ư ����

    ImFont* UnicodeFont = io.Fonts->Fonts[FEATHER_FONT];

    ImVec2 ControlButtonSize = ImVec2(32, 32);

    ImGui::PushFont(UnicodeFont);
    ImGui::Button(u8"\ue9bc", ControlButtonSize); // MOVE

    ImGui::SameLine(0, 5.0f);

    ImGui::Button(u8"\ue9d3", ControlButtonSize); // ROTATE

    ImGui::SameLine(0, 5.0f);

    ImGui::Button(u8"\ue9ab", ControlButtonSize); // SCALE

    ImGui::SameLine();
    // â ������ ��ü ������ ���� �ʺ� �����ɴϴ�.
    float windowContentWidth = ImGui::GetWindowContentRegionMax().x;

    // �� ��ư�� ��ư ���� ������ �� �ʺ� ����մϴ�.
    float buttonsTotalWidth = ControlButtonSize.x * 2 + 5.0f;

    // ������ ������ ���� ��ư���� ���۵� X ��ǥ�� ����մϴ�.
    float posX = windowContentWidth - buttonsTotalWidth;
    if (posX < 0)
        posX = 0;

    // ���� Ŀ�� ��ġ�� Y�� �״�� �����ϰ�, X ��ǥ�� ���������� �̵��մϴ�.
    ImGui::SetCursorPosX(posX);

    if (ImGui::Button(u8"\ue9b7", ControlButtonSize))
    {
        auto Window = UEditorDesigner::Get().GetWindow("ConsoleWindow");
        if (Window)
        {
            // dynamic_cast�� ���� MyWindow Ÿ������ ��ȯ �� setter ȣ��
            if (ISwitchable* Console = dynamic_cast<ISwitchable*>(Window.get()))
            {
                Console->Toggle();
            }
        }
    }

    ImGui::SameLine(0, 5.0f);

    if (ImGui::Button(u8"\ue918", ControlButtonSize))
    {
        auto Window = UEditorDesigner::Get().GetWindow("StatWindow");
        if (Window)
        {
            // dynamic_cast�� ���� MyWindow Ÿ������ ��ȯ �� setter ȣ��
            if (ISwitchable* Stat = dynamic_cast<ISwitchable*>(Window.get()))
            {
                Stat->Toggle();
            }
        }
    }

    ImGui::PopFont();

    // ��Ʈ�� ��ư ����
	ImGui::Separator(); // ���� ���м�
    // ���� ����

	const char* items[] = { "Cube", "Sphere", "Triangle" };
    ImGui::SetNextItemWidth(100); // ���ϴ� �ʺ�
	ImGui::Combo("Primitive", &PrimtiveTypeNumber, items, IM_ARRAYSIZE(items));
    
    ImGui::SameLine(0, 5.0f);

    CreateCustomInputInt("Number Of Spawn", ImGuiDataType_S32, &SpawnNumber, "%d", ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsDecimal);

    // ���� ����
    ImGui::Separator(); // ���� ���м�
    // �� ����

    ImGui::PushFont(UnicodeFont);
    
    ImGui::Button(u8"\ue96d", ControlButtonSize); // New Scene
    ImGui::SameLine(0, 5.0f);
    ImGui::Button(u8"\ue950", ControlButtonSize); // Load Scene
    ImGui::SameLine(0, 5.0f);
    ImGui::Button(u8"\ue9d6", ControlButtonSize); // Save Scene
    
    ImGui::PopFont();

    char buf[20];
    strncpy_s(buf, SceneName.c_str(), sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';

    ImGui::SetNextItemWidth(150); // ���ϴ� �ʺ�

    if (ImGui::InputText("SceneName", buf, sizeof(buf)))
    {
        SceneName = buf;
    }

    // �� ����
    ImGui::Separator(); // ���� ���м�

    ImGui::Checkbox("Orthogonal", &bIsOrthogonal);

    ImGui::DragFloat("FOV", &FOV, 1, 0, 120);

    if (ImGui::DragFloat3("Camera Location", Location))
    {
        CameraLocation.X = Location[0];
        CameraLocation.Y = Location[1];
        CameraLocation.Z = Location[2];
    }

    if (ImGui::DragFloat3("Camera Rotation", Rotation))
    {
        CameraRotation.X = Rotation[0];
        CameraRotation.Y = Rotation[1];
        CameraRotation.Z = Rotation[2];
    }

	ImGui::End();
}

bool ControlWindow::CreateCustomInputInt(const char* label, ImGuiDataType data_type, void* p_data, const char* format, ImGuiInputTextFlags flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    IM_ASSERT((flags & ImGuiInputTextFlags_EnterReturnsTrue) == 0); // Not supported by InputScalar()!

    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

    void* p_data_default = (g.NextItemData.HasFlags & ImGuiNextItemDataFlags_HasRefVal) ? &g.NextItemData.RefVal : &g.DataTypeZeroValue;

    char buf[64];
    if ((flags & ImGuiInputTextFlags_DisplayEmptyRefVal) && ImGui::DataTypeCompare(data_type, p_data, p_data_default) == 0)
        buf[0] = 0;
    else
        ImGui::DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, p_data, format);

    // Disable automatic MarkItemEdited(), we'll call it manually.
    g.NextItemData.ItemFlags |= ImGuiItemFlags_NoMarkEdited;
    flags |= ImGuiInputTextFlags_AutoSelectAll | (ImGuiInputTextFlags)ImGuiInputTextFlags_LocalizeDecimalPoint;

    bool value_changed = false;
    const float button_size = ImGui::GetFrameHeight();

    ImGui::BeginGroup(); // �׷����� ���� �ܺο��� ������ ������ ���� �� �ְ� ��.
    ImGui::PushID(label);
    ImGui::SetNextItemWidth(ImMax(1.0f, ImGui::CalcItemWidth() - (button_size + style.ItemInnerSpacing.x) * 6 + 15));
    if (ImGui::InputText("", buf, IM_ARRAYSIZE(buf), flags))
        value_changed = ImGui::DataTypeApplyFromText(buf, data_type, p_data, format, (flags & ImGuiInputTextFlags_ParseEmptyRefVal) ? p_data_default : NULL);
    IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Inputable);

    // ������ Step ��ư(-, +) �κ��� �����ϰ�, ��� "spawn" ��ư�� �߰��մϴ�.
    const ImVec2 backup_frame_padding = style.FramePadding;
    style.FramePadding.x = style.FramePadding.y;
    if (flags & ImGuiInputTextFlags_ReadOnly)
        ImGui::BeginDisabled();
    ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    if (ImGui::ButtonEx("Spawn", ImVec2(button_size * 4.2, button_size)))
    {
        // "spawn" ��ư Ŭ�� �� ���ϴ� ������ �����մϴ�.
        // ���� ���, Ư���� ������ �����ϰų� ���¸� ������ �� �ֽ��ϴ�.
        value_changed = true;
    }
    ImGui::PopItemFlag();
    if (flags & ImGuiInputTextFlags_ReadOnly)
        ImGui::EndDisabled();

    style.FramePadding = backup_frame_padding;

    ImGui::PopID();
    ImGui::EndGroup();
        

    g.LastItemData.ItemFlags &= ~ImGuiItemFlags_NoMarkEdited;
    if (value_changed)
        ImGui::MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}
