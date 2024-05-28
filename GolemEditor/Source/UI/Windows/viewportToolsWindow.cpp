#include "UI/Windows/viewportToolsWindow.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Resource/sceneManager.h"
#include "Utils/viewportTools.h"


ViewportToolsWindow::ViewportToolsWindow(std::string _name)
    : Window(_name)
{}

ViewportToolsWindow::~ViewportToolsWindow() {}

void ViewportToolsWindow::Update()
{
    ImGui::Begin(name.c_str());
    ImGui::Checkbox("Wirefram view", &isWireframeViewActive);
    ImGui::Checkbox("Terrain gradient view", &isTerrainGradientViewActive);
    ImGui::End();

    ViewportTools::currentViewMode = isWireframeViewActive ? WIREFRAME : DEFAULT;    // Set the view mode depending on isWireframeViewActive
    // Set the terrain view to gradient
    for (int i = 0; i < SceneManager::GetCurrentScene()->gameObjects.size(); i++)
    {
        Terrain* terrain = dynamic_cast<Terrain*>( SceneManager::GetCurrentScene()->gameObjects[i]);
        if (terrain)
        {
            if (isTerrainGradientViewActive)
            {
                terrain->useGradient = true;
            }
            else
            {
                terrain->useGradient = false;
            }
        }
    }
}