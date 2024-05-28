#include "golemEngine.h"

#include "Wrappers/graphicWrapper.h"
#include "Wrappers/windowWrapper.h"
#include "Resource/sceneManager.h"
#include "Core/scene.h"
#include "Core/camera.h"
#include "Inputs/inputManager.h"
#include "vector4.h"
#include "Reflection/classesManager.h"


void GolemEngine::PerformComputShaderComputations()
{
}

void GolemEngine::Init()
{
    ClassesManager::AddAllClasses();
    ResourceManager::CreateAndLoadResources();
    m_physicSystem = new PhysicSystem();
    SceneManager::Init();
    InputManager::Init(WindowWrapper::window);
    m_camera = new Camera();
    m_playerCamera = new Camera();
}

void GolemEngine::UpdateDeltaTime()
{
    static float lastFrame = 0;
    m_deltaTime = static_cast<float>(glfwGetTime()) - lastFrame;
    lastFrame = static_cast<float>(glfwGetTime());
}

void GolemEngine::ProcessInput()
{
}

void GolemEngine::Update()
{
    UpdateDeltaTime();
    // Bind next framebuffer to the scene buffer
    GraphicWrapper::BindFramebuffer();
    // Assign background color and clear previous scene buffers
    GraphicWrapper::SetBackgroundColor(Vector4(0.f, 0.f, 0.f, 1.0f));
    // Clear buffer
    GraphicWrapper::ClearBuffer();
    // Render the scene to the framebuffer
    SceneManager::GetCurrentScene()->Update(m_camera);
    // Go back to original framebuffer
    GraphicWrapper::UnbindFramebuffer();
    
   
    GraphicWrapper::BindPlaySceneFrambuffer();
    GraphicWrapper::SetBackgroundColor(Vector4(0.f, 0.f, 0.f, 1.0f));
    GraphicWrapper::ClearBuffer();
    SceneManager::GetCurrentScene()->Update(m_playerCamera);
    GraphicWrapper::UnbindFramebuffer();
}

void GolemEngine::Close()
{
    delete m_physicSystem;
    glfwTerminate();
}

Camera* GolemEngine::GetCamera()
{
    return m_camera;
}

Camera* GolemEngine::GetPlayerCamera()
{
    return m_playerCamera;
}

float GolemEngine::GetDeltaTime()
{
    return m_deltaTime;
}

float GolemEngine::GetTime()
{
    return static_cast<float>(glfwGetTime());
}

bool GolemEngine::GetGameMode()
{
    return m_gameMode;
}

void GolemEngine::StartGameMode()
{
    m_gameMode = true;
}

void GolemEngine::StopGameMode()
{
    m_gameMode = false;
}
