#include "UI/Windows/worldBuilderWindow.h"

#include "golemEngine.h"
#include "WorldBuilder/worldBuilder.h"
#include "Resource/sceneManager.h"
#include "Utils/tools.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Resource/Rendering/skybox.h"

const std::string CREATE_DEFAULT_TERRAIN_POPUP_TITLE = "Specify new terrain dimensions";
const std::string CREATE_DEFAULT_TERRAIN_POPUP_MESSAGE = "Enter terrain dimensions";
const std::string CREATE_DEFAULT_NOISEMAP_TERRAIN_POPUP_TITLE = "Select a heightmap";
const std::string CREATE_DEFAULT_NOISEMAP_TERRAIN_POPUP_MESSAGE = "Available heightmaps : ";


WorldBuilderWindow::WorldBuilderWindow(std::string _name) 
    : Window(_name)
{}

WorldBuilderWindow::~WorldBuilderWindow() {}


void WorldBuilderWindow::Update()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 10.0f));

    ImGui::Begin(name.c_str());

    if (ImGui::Button("Create new terrain"))
    {
        m_isCreateDefaultTerrainPopupActive = true;
    }
    if (m_isCreateDefaultTerrainPopupActive)
    {
        UpdateCreateDefaultTerrainPopup(m_newDefaultTerrainResolutionX, m_newDefaultTerrainResolutionZ);
    }

    if (ImGui::Button("Create new noisemap terrain"))
    {
        m_isCreateDefaultNoisemapTerrainPopupActive = true;
    }
    if (m_isCreateDefaultNoisemapTerrainPopupActive)
    {
        UpdateCreateDefaultNoisemapeTerrainPopup();
    }

    if (dynamic_cast<Terrain*>(GolemEngine::selectedGameObject))
    {
        Terrain* selectedTerrain = reinterpret_cast<Terrain*>(GolemEngine::selectedGameObject);
        
        if (ImGui::Button("Calculate normals"))
        {
            selectedTerrain->CalculateNormals();
        }

        ImGui::Checkbox("edit mode", &WorldBuilder::brush->isActive);
        if (WorldBuilder::brush->isActive)
        {
            ImGui::Text("Tetxure layers");
            ImGui::SliderFloat("texture1 level", &WorldBuilder::texture1Level, 0.0f, selectedTerrain->yMax);
            ImGui::SliderFloat("texture2 level", &WorldBuilder::texture2Level, 0.0f, selectedTerrain->yMax);
        }
        
        if (WorldBuilder::brush->isActive)
        {
            ImGui::Text("Brush settings");
            ImGui::SliderFloat("Force", &WorldBuilder::brush->force, 0.0f, 1.0f);
            ImGui::SliderFloat("Radius", &WorldBuilder::brush->radius, 0.1f, 1.0f);

            selectedTerrain->GetComputeShader()->SetBool("isBrushActive", true);
            selectedTerrain->GetComputeShader()->SetFloat("brushRadius", WorldBuilder::brush->radius);
            selectedTerrain->GetComputeShader()->SetFloat("brushForce", WorldBuilder::brush->force);
        }
        else
        {
            selectedTerrain->GetComputeShader()->SetBool("isBrushActive", false);
        }
    }

    if (ImGui::TreeNode("Skybox"))
    {
        if (ImGui::Button("Space"))
        {
            Skybox::GetInstance().SetSpaceSkybox();
        }
        if (ImGui::Button("Blue Sky"))
        {
            Skybox::GetInstance().SetTexture();
        }
        ImGui::TreePop();
    }


    ImGui::End();

    ImGui::SetWindowFontScale(1.0f);
    
    ImGui::PopStyleVar();
}

void WorldBuilderWindow::UpdateCreateDefaultTerrainPopup(int& _xResolution, int& _zResolution)
{
    ImGui::OpenPopup(CREATE_DEFAULT_TERRAIN_POPUP_TITLE.c_str());
    
    // Popup content
    if (ImGui::BeginPopupModal(CREATE_DEFAULT_TERRAIN_POPUP_TITLE.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(CREATE_DEFAULT_TERRAIN_POPUP_MESSAGE.c_str());

        // Set terrain dimensions. Resolution are for the number of vertices inside the terrain
        ImGui::InputInt("x resolution", &_xResolution, 0, 0);
        ImGui::InputInt("z resolution", &_zResolution, 0, 0);

        // Check if user inputs are valid
        bool isUserInputValid = true;
        if (_xResolution < 2 || _zResolution < 2)    // Minimum terrain dimension
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Warning: Minimum terrain resolution are 2 by 2.");
            isUserInputValid = false;
        }

        // Create terrain on valid input and "OK" button pressed
        if (isUserInputValid && ImGui::Button("OK"))
        {
            WorldBuilder::CreateDefaultTerrain(_xResolution, _zResolution);
            m_isCreateDefaultTerrainPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            m_isCreateDefaultTerrainPopupActive = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void WorldBuilderWindow::UpdateCreateDefaultNoisemapeTerrainPopup()
{
    ImGui::OpenPopup(CREATE_DEFAULT_NOISEMAP_TERRAIN_POPUP_TITLE.c_str());
    
    // Popup content
    if (ImGui::BeginPopupModal(CREATE_DEFAULT_NOISEMAP_TERRAIN_POPUP_TITLE.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(CREATE_DEFAULT_NOISEMAP_TERRAIN_POPUP_MESSAGE.c_str());

        for (int i = 0; i < Tools::GetFolderSize(Tools::FindFolder("Heightmaps")); i++)
        {
            std::string heightmapName = Tools::GetFolderElementsNames(Tools::FindFolder("Heightmaps"))[i];
            if (ImGui::MenuItem(heightmapName.c_str()))
            {
                m_selectedHeightmap = heightmapName;
            }
        }

        // Check if user inputs are valid
        bool isUserInputValid = false;
        if (m_selectedHeightmap != "")
        {
            isUserInputValid = true;
            ImGui::NewLine();
            std::string text = "selected heighmtap : " + m_selectedHeightmap; 
            ImGui::Text(text.c_str());
        }

        // Create terrain on valid input and "OK" button pressed
        if (isUserInputValid && ImGui::Button("OK"))
        {
            WorldBuilder::CreateNoisemapTerrain(Tools::FindFile(m_selectedHeightmap).c_str());
            m_isCreateDefaultNoisemapTerrainPopupActive = false;
            ImGui::CloseCurrentPopup();
        }
        if (isUserInputValid)
        {
            ImGui::SameLine();
        }
        if (ImGui::Button("Cancel"))
        {
            m_isCreateDefaultNoisemapTerrainPopupActive = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}