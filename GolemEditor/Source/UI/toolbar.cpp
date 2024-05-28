#include "UI/toolbar.h"
#include <iostream>
#include "Resource/sceneManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Resource/Rendering/skybox.h"


void Toolbar::Update()
{
    if (ImGui::BeginMenuBar())
    {
        // Menu widget
        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::EndMenu();
        }

        // Scenes widget
        if (ImGui::BeginMenu("Scenes"))
        {
            if (ImGui::MenuItem("Save scene"))
            {
                SceneManager::SaveScene();
            }

            if (ImGui::BeginMenu("Load scenes"))
            {
                // Get the amount of scenes in the Scenes folder
                for (int i = 0; i < SceneManager::GetSceneCount(); i++)
                {
                    if (SceneManager::GetScene(i)->name != SceneManager::GetCurrentScene()->name)
                    {
                        std::string sceneName = SceneManager::GetScene(i)->name;
                        if (ImGui::MenuItem(sceneName.c_str()))
                        {
                            SceneManager::LoadScene(i);
                        }
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Create scene"))
            {
                std::string sceneName = "scene_";
                sceneName.append(std::to_string(SceneManager::GetSceneCount()));
                SceneManager::CreateScene(sceneName);
            }
            
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }
}
