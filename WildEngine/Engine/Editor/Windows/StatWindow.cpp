#include "StatWindow.h"
#include "../../../ImGui/imgui.h"

#include "Types/Types.h"
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

    UINT32 Objects, Memory;

    UObjectManager::GetInstance().PrintMemoryUsage(Objects, Memory);

    ImGui::Text("Spawnned Objects: %d", Objects);
  
    ImGui::Text("Memory Usage: %d", Memory);

    ImGui::End();
}

void StatWindow::Toggle()
{
    bWasOpen = !bWasOpen;
}