#include "UI/Windows/viewport.h"

#include "Core/camera.h"
#include "golemEngine.h"
#include "Wrappers/graphicWrapper.h"
#include "Wrappers/windowWrapper.h"
#include "Resource/Rendering/texture.h"
#include "Core/mesh.h"
#include "Resource/sceneManager.h"
#include "Utils/tools.h"
#include "Inputs/inputManager.h"
#include "UI/EditorUi.h"
#include "vector4.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "MathsLib/utils.h"
#include "Components/transform.h"
#include "ImGuizmo.h"
#include "UI/Windows/playScene.h"
#include "WorldBuilder/worldBuilder.h"

bool g_isFromFileBrowser = false;

Viewport::Viewport(std::string _name)
    : Window(_name), m_lastX(0), m_lastY(0), m_yaw(0), m_pitch(0)
{
}

Viewport::~Viewport() {}

void Viewport::Update()
{
    SetCamera(GolemEngine::GetCamera());

    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoMove);   // To make the window not movable because otherwise mouse position won't work if out of window

    auto viewportOffset = ImGui::GetCursorPos();

    auto windowSize = ImGui::GetWindowSize();
   

    /*std::cout << ImGui::GetWindowPos().x << std::endl;
    std::cout << ImGui::GetWindowPos().y << std::endl;*/

    ImVec2 minBound = ImGui::GetWindowPos(); 

    minBound.x += viewportOffset.x;
    minBound.y += viewportOffset.y;

    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
    m_viewportBounds[0] = { minBound.x, minBound.y };
    m_viewportBounds[1] = { maxBound.x, maxBound.y };

    auto [mx, my] = ImGui::GetMousePos();
    mx -= m_viewportBounds[0].x;
    my -= m_viewportBounds[0].y;

    Vector2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
    my = viewportSize.y - my; 

    int mouseX = (int)mx;
    int mouseY = (int)my;

    GolemEngine::mouseViewportPos = {(float)mouseX, (float)mouseY};

    std::vector<GameObject*> objects = SceneManager::GetCurrentScene()->GetGameObjects();

    if (!ImGuizmo::IsOver())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            if (WorldBuilder::brush->isActive)
            {
                Vector2 brushUv = GraphicWrapper::ReadPixelFromUVBuffer(mouseX * 1636 / 1210, ((mouseY - 32) * 980 / 700));    // Mouse and framebuffer are not to the size of the viewport
                if (brushUv.x != 0.0f && brushUv.y != 0.0f)
                {
                    Terrain* selectedTerrain = reinterpret_cast<Terrain*>(GolemEngine::selectedGameObject);
                    selectedTerrain->UseComputeShader(brushUv);
                    selectedTerrain->RetrieveComputeData(m_camera);
                }
            }
            else
            {
                GraphicWrapper::AttachTexture(GL_RED_INTEGER, GraphicWrapper::textures[1]->m_width, GraphicWrapper::textures[1]->m_height, GL_COLOR_ATTACHMENT0 + 1, GraphicWrapper::textures[1]->id, GraphicWrapper::GetFbo());
                int pixelData = GraphicWrapper::ReadPixelFromIndexBuffer(mouseX, mouseY);
                GraphicWrapper::AttachTexture(GL_RGBA, GraphicWrapper::textures[0]->m_width, GraphicWrapper::textures[0]->m_height, GL_COLOR_ATTACHMENT0, GraphicWrapper::textures[0]->id, GraphicWrapper::GetFbo());
    
                for (int i = 0; i < objects.size(); i++)
                {
                    if (objects[i]->GetId() == pixelData)
                    {
                        EditorUi::selectedGameObject = objects[i];
                    }
    
                    else if (pixelData == -1)
                    {
                        EditorUi::selectedGameObject = nullptr;
                    }
                }
            }
        }
    }
    
    ImGui::Image((ImTextureID)GraphicWrapper::textures[0]->id, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));

    Vector4 windowDimensions(ImGui::GetWindowDockNode()->Pos.x, ImGui::GetWindowDockNode()->Size.x, ImGui::GetWindowDockNode()->Pos.y, ImGui::GetWindowDockNode()->Size.y);

    DragDropModel();

    if (ImGui::IsWindowHovered() && InputManager::IsButtonPressed(BUTTON_1))
    {
        m_lastSpacePress = true;
        m_camera->ProcessKeyboardInput(GolemEngine::GetDeltaTime());
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        // Update camera with mouse position
        m_camera->ProcessMouseMovement(InputManager::GetMouseWindowPos(), true, windowDimensions, (int)ImGui::GetMousePos().x, (int)ImGui::GetMousePos().y);
        // Update camera speed depending on scroll
        m_camera->ProcessMouseScroll(InputManager::GetMouseScroll());
        InputManager::SetMouseScroll(0.0f);     // Otherwise the camera will continue to change since GetMouseScroll value doesn't change but has a value
    }

    // Upon space key released
    if (m_lastSpacePress && !InputManager::IsButtonPressed(BUTTON_1))
    {
        m_lastSpacePress = false;
        m_camera->isFirstMouse = true;  // Important so the next time you move in the viewport, it doesn't teleport the camera to the cursor
    }

    if (EditorUi::selectedGameObject)
    {
        EditorUi::selectedGameObject->transform->EditTransformGizmo();
    }

    ImGui::End();
}

void Viewport::SetCamera(Camera* _camera)
{
    m_camera = _camera;
}

void Viewport::DragDropModel()
{
    if (ImGui::BeginDragDropTarget())
    {
        m_isDragging = true;

        ImGui::EndDragDropTarget();
    }

    if (g_isFromFileBrowser && m_isDragging)
    {
        ImVec2 itemRectMin = ImGui::GetItemRectMin();
        ImVec2 itemRectMax = ImGui::GetItemRectMax();

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileDrop"))
        {
            std::string droppedFilePath(static_cast<const char*>(payload->Data), payload->DataSize);
            SceneManager::GetCurrentScene()->CreateNewModel(droppedFilePath);
            SceneManager::GetCurrentScene()->isNewObjectDropped = true;
            m_isDragging = false;
            g_isFromFileBrowser = false;
        }
    }
}

Vector2 Viewport::GetViewportSize()
{
    return Vector2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

Camera* Viewport::GetCamera()
{
    return m_camera;
}
