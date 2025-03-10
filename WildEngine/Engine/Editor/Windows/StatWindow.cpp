#include "StatWindow.h"
#include "../../../ImGui/imgui.h"
#include "Object/ObjectManager.h"

StatWindow::StatWindow()
{
    bWasOpen = true;
}

void StatWindow::Render()
{
    if (!bWasOpen)
    {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    // ���ϴ� ���� ������ ����
    float normalHeight = 250.0f;

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 245, io.DisplaySize.y - normalHeight - 5), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, normalHeight), ImGuiCond_Always);

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse;

    if (!ImGui::Begin("Stat", &bWasOpen, WindowFlags))
    {
        ImGui::End(); // �ݵ�� ȣ���ؾ� ��
        return;
    }

    ImGui::Text(u8"Spawnned Objects: %d", UObjectManager::GetInst().GetTotalAllocationCount());
    
    ImGui::Text(u8"Memory Usage: %d bytes", UObjectManager::GetInst().GetTotalAllocationBytes());

    ImGui::SameLine(0, 5.0f);

    ImGui::End();
}

void StatWindow::Toggle()
{
    bWasOpen = !bWasOpen;
}