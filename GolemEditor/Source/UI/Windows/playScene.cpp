#include "UI/Windows/playScene.h"
#include "Wrappers/graphicWrapper.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "golemEngine.h"
#include "Inputs/inputManager.h"

bool g_isPlayTesting = false;

PlayScene::PlayScene(std::string _name)
	:Window(_name)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 1, 0, 1));

    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
       
    if (ImGui::Button("Play"))
    {
        g_isPlayTesting = true;
        GolemEngine::StartGameMode();
    }
    
    Vector4 windowDimensions(ImGui::GetWindowDockNode()->Pos.x, ImGui::GetWindowDockNode()->Size.x, ImGui::GetWindowDockNode()->Pos.y, ImGui::GetWindowDockNode()->Size.y);


    ImGui::Image((ImTextureID)GraphicWrapper::GetPlaySceneId(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
    if (g_isPlayTesting)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        GolemEngine::GetPlayerCamera()->ProcessMouseMovement(InputManager::GetMouseWindowPos(), true, windowDimensions, (int)ImGui::GetMousePos().x, (int)ImGui::GetMousePos().y);
        if (ImGui::SmallButton("X") || InputManager::IsKeyPressed(KEY_ESCAPE))
        {
            g_isPlayTesting = false;
            GolemEngine::StopGameMode();
        }
    }
    ImGui::PopStyleColor();
    ImGui::End();
    m_lastMousepos = Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
}