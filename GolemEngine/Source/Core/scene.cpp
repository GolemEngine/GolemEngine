#include "Core/scene.h"

#include <filesystem>
#include <nlohmann/json.hpp>

#include "golemEngine.h"
#include "Core/gameobject.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Components/Light/pointLight.h"
#include "Components/Light/directionalLight.h"
#include "Components/Light/spotLight.h"
#include "Components/transform.h"
#include "Components/meshRenderer.h"
#include "Components/audio.h"
#include "Components/Physic/sphereCollider.h"
#include "Components/Physic/boxCollider.h"
#include "Resource/Rendering/model.h"
#include "Resource/Rendering/texture.h"
#include "Resource/Rendering/skybox.h"
#include "Resource/resourceManager.h"
#include "Resource/sceneManager.h"
#include "Physic/physicSystem.h"
#include "utils.h"
#include "Utils/tools.h"
#include "Wrappers/windowWrapper.h"

using json = nlohmann::json;


Scene::Scene(std::string _name, bool _makeSceneEmpty)
    : name(_name)
{
    SceneManager::SetCurrentScene(this);
    m_world = new GameObject("World", new Transform(Vector3(0, 0, 0), Vector3(0), Vector3(1), nullptr));
    InitLights();

    
    if (!_makeSceneEmpty)    // If there is already a save of the world, don't init a default scene
    { 
        InitDefaultScene();
    }
}

void Scene::Test(Collider* _collider, Collider* _other)
{
    PhysicSystem::SetVelocity(_collider->id, Vector3(0, 10, 0));
}


void Scene::InitDefaultScene()
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();
    Shader* defaultShader = resourceManager->Get<Shader>(ResourceManager::GetDefaultShader());

    // Create a viking room model
    std::string vikingName = "viking";
    Transform* vikingTransform = new Transform(Vector3(0, 0, -5), Vector3(0), Vector3(1), m_world->transform);
    GameObject* vikingGo = new GameObject(vikingName, vikingTransform);
    Texture* vikingText = resourceManager->Get<Texture>("viking_room.jpg");
    Model* vikingRoom = resourceManager->Get<Model>("viking_room.obj");
    Mesh* vikingMesh = new Mesh(vikingRoom, vikingText, defaultShader);
    vikingGo->AddComponent(new MeshRenderer(vikingMesh));
    Audio* audio1 = new Audio("music_01.wav");
    vikingGo->AddComponent(audio1);

    std::string vikingName2 = "viking2";
    Transform* vikingTransform2 = new Transform(Vector3(0, -3, -5), Vector3(0), Vector3(1), m_world->transform);
    GameObject* vikingGo2 = new GameObject(vikingName2, vikingTransform2);
    Texture* vikingText2 = resourceManager->Get<Texture>("aoi_todo.jpg");
    Model* vikingRoom2 = resourceManager->Get<Model>("cube.obj");
    Mesh* vikingMesh2 = new Mesh(vikingRoom2, vikingText2, defaultShader);
    vikingGo2->AddComponent(new MeshRenderer(vikingMesh2));

    // Create a sphere model
    std::string ballBaldName = "ball_bald";
    Transform* ballBaldTransform = new Transform(Vector3(0, 3, -5), Vector3(0), Vector3(1), m_world->transform);
    GameObject* ballBaldGo = new GameObject(ballBaldName, ballBaldTransform);
    Texture* ballBaldTexture = resourceManager->Get<Texture>("aoi_todo.jpg");
    Model* ballBald = resourceManager->Get<Model>("sphere.obj");
    Mesh* ballBaldMesh = new Mesh(ballBald, ballBaldTexture, defaultShader);
    ballBaldGo->AddComponent(new MeshRenderer(ballBaldMesh));
    SphereCollider* sc = new SphereCollider;
    ballBaldGo->AddComponent(sc);
    sc->Begin();

    std::string ballBaldName2 = "ball_bald2";
    Transform* ballBaldTransform2 = new Transform(Vector3(-3, 0, 0), Vector3(0), Vector3(1), m_world->transform);
    GameObject* ballBald2Go = new GameObject(ballBaldName2, ballBaldTransform2);
    Texture* ballBaldTexture2 = resourceManager->Get<Texture>("all_bald.jpg");
    Model* ballBald2 = resourceManager->Get<Model>("sphere.obj");
    Mesh* ballBaldMesh2 = new Mesh(ballBald2, ballBaldTexture2, defaultShader);
    ballBald2Go->AddComponent(new MeshRenderer(ballBaldMesh2));
    
    sc->onCollisionEnter = [this](Collider* _collider, Collider* _other) -> void { Test(_collider, _other); };
}

void Scene::InitLights()
{
    // Set up directional light
    DirectionalLight* dir = new DirectionalLight;
    m_world->AddComponent(dir);
}

void Scene::Update(Camera* _camera)
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    // Skybox shader load
    Shader* skyboxShader = resourceManager->Get<Shader>("skybox_shader");
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader->Use();
    Matrix4 viewMatrix = _camera->GetViewMatrix();
    Matrix4 view = viewMatrix.ExtractRotationAndScale(viewMatrix);
    Matrix4 projection = Matrix4::Projection(DegToRad(_camera->GetZoom()), WindowWrapper::GetScreenSize().x  / WindowWrapper::GetScreenSize().y, _camera->GetNear(), _camera->GetFar());
    skyboxShader->GetVertexShader()->SetMat4("view", view);
    skyboxShader->GetVertexShader()->SetMat4("projection", projection);
    // skybox cube
    glBindVertexArray(Skybox::GetInstance().GetSkyboxVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::GetInstance().GetSkyboxCubeMapId());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    Shader* defaultShader = resourceManager->Get<Shader>(ResourceManager::GetDefaultShader());
    defaultShader->Use();


    if (GolemEngine::GetGameMode() && _camera == GolemEngine::GetPlayerCamera())
    {
        PhysicSystem::PreUpdate();
        PhysicSystem::Update();
        PhysicSystem::PostUpdate();
    }

    UpdateGameObjects(_camera);    // Always at least one gameobject (world)

    if (!m_dirLights.empty() || !m_pointLights.empty() || !m_spotLights.empty())
    {
        UpdateLights(defaultShader);
    }
    for (int i = 0; i < m_deletedGameObjects.size(); i++)
    {
        delete m_deletedGameObjects[i];
    }
    m_deletedGameObjects.clear();
}

void Scene::UpdateGameObjects(Camera* _camera)
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();
    Shader* shader = resourceManager->Get<Shader>("default_shader");

    if (isNewObjectDropped)
    {
        CreateNewObject(loadingObject.c_str(), loadingObject.c_str());
        isNewObjectDropped = false;
    }

    m_world->transform->UpdateSelfAndChilds();

    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->Update();
        shader->GetFragmentShader()->SetInt("entityID", (int)gameObjects[i]->GetId());

        if (MeshRenderer* meshRenderer = gameObjects[i]->GetComponent<MeshRenderer>())
        {
            meshRenderer->Draw(_camera);
        }

        if (!GolemEngine::GetGameMode())
        {
            Collider* collider = gameObjects[i]->GetComponent<Collider>();
            if (collider && collider->owner->IsSelected)
                collider->Draw(_camera);
        }

        if (gameObjects[i]->isTerrain)
        {
            Terrain* terrain = dynamic_cast<Terrain*>(gameObjects[i]);
            if (terrain)
            {
                terrain->Draw(_camera);
        
                if (GolemEngine::selectedGameObject == terrain)    // Use compute shader only if terrain is being selected
                {
                    // terrain->RetrieveComputeData();
                    // terrain->UpdateVertices(_camera);
                }
            }
        }
    }
   
}

void Scene::UpdateLights(Shader* _shader)
{
    _shader->Use();

    _shader->GetFragmentShader()->SetInt("nbrDirLights", (int)m_dirLights.size());
    _shader->GetFragmentShader()->SetInt("nbrPointLights", (int)m_pointLights.size());
    _shader->GetFragmentShader()->SetInt("nbrSpotLights", (int)m_spotLights.size());

    for (unsigned int i = 0; i < m_dirLights.size(); ++i)
    {
        m_dirLights[i]->SetDirectionalLight(_shader);
    }
    for (unsigned int i = 0; i < m_pointLights.size(); ++i)
    {
        m_pointLights[i]->SetPointLight(_shader);
    }
    for (unsigned int i = 0; i < m_spotLights.size(); ++i)
    {
        m_spotLights[i]->SetSpotLight(_shader);
    }
}

bool Scene::IsNameExists(const std::string& _name)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i]->name == _name)
        {
            return true;
        }
    }
    return false;
}

void Scene::CreateNewObject(std::string _name, std::string _modelName, std::string _textureName, std::string _shaderName)
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    std::string name = _name;
    Transform* transform = new Transform(Vector3(1), Vector3(0), Vector3(1), m_world->transform);
    Texture* texture;
    Shader* shader;

    if (_textureName.empty())
    {
        texture = resourceManager->Get<Texture>(ResourceManager::GetDefaultTexture());
    }
    else
    {
        texture = resourceManager->Get<Texture>(_textureName);
    }

    if (_shaderName.empty())
    {
        shader = resourceManager->Get<Shader>(ResourceManager::GetDefaultShader());
    }
    else
    {
        shader = resourceManager->Get<Shader>(_shaderName);
    }

    // Using the rename functions
    int suffix = 2; // start at 2 because of two objects having the same name
    std::string originalName = name;
    while (IsNameExists(name))    // If an object already has the same name, add "_[suffix]" to it to make new name
    {
        name = originalName + "_" + std::to_string(suffix++);
    }

    Model* model = resourceManager->Get<Model>(_modelName);
    Mesh* mesh = new Mesh(model, texture, shader);
    GameObject* gameObject = new GameObject(name, transform);
    gameObject->AddComponent(new MeshRenderer(mesh));
    gameObjects.push_back(gameObject);
}

void Scene::CreateNewModel(std::string _filePath, std::string _resourceName)
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    bool hasDuplicate = false;
    for (const auto& pair : resourceManager->GetResources())
    {
        if (pair.first == GetFileName(_filePath))
        {
            hasDuplicate = true;
            loadingObject = GetFileName(_filePath);
            break;
        }
    }

    if (hasDuplicate)
    {
        std::cout << "Resource with name " << _resourceName << " already exists." << std::endl;
        return;
    }

    if (_resourceName == "")
    {
        Model* model = resourceManager->Get<Model>(ResourceManager::GetDefaultModel());
        loadingObject = GetFileName(_filePath) + ".obj";
    }
    else
    {
        Model* model = resourceManager->Get<Model>(_resourceName);
        loadingObject = GetFileName(_filePath) + ".obj";
    }
}

void Scene::AddDeletedGameObject(GameObject* _gameObject)
{
    m_deletedGameObjects.push_back(_gameObject);
}


void Scene::RemoveGameObject(GameObject* _gameObject)
{
    bool removed = false;
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i] == _gameObject)
        {
            removed = true;
        }
        if (removed)
        {
            gameObjects[i]->SetId(i - 1);
        }
    }
    std::erase(gameObjects, _gameObject);
}

void Scene::AddLight(Light* _light)
{
    if (PointLight* pL = dynamic_cast<PointLight*>(_light))
    {
        m_pointLights.push_back(pL);
    }
    else if (SpotLight* sL = dynamic_cast<SpotLight*>(_light))
    {
        m_spotLights.push_back(sL);

    }
    else if (DirectionalLight* dL = dynamic_cast<DirectionalLight*>(_light))
    {
        m_dirLights.push_back(dL);
    }
}

void Scene::DeleteLight(Light* _light)
{
    if (PointLight* pL = dynamic_cast<PointLight*>(_light))
    {
        std::erase(m_pointLights, pL);
    }
    else if (SpotLight* sL = dynamic_cast<SpotLight*>(_light))
    {
        std::erase(m_spotLights, sL);
    }
    else if (DirectionalLight* dL = dynamic_cast<DirectionalLight*>(_light))
    {
        std::erase(m_dirLights, dL);
    }
}

const std::vector<DirectionalLight*>& Scene::GetDirectionalLights()
{
    return m_dirLights;
}

const std::vector<PointLight*>& Scene::GetPointLights()
{
    return m_pointLights;
}

const std::vector<SpotLight*>& Scene::GetSpotLights()
{
    return m_spotLights;
}

size_t Scene::GetMaxDirectionalLights()
{
    return m_maxDirLights;
}

size_t Scene::GetMaxPointLights()
{
    return m_maxPointLights;
}

size_t Scene::GetMaxSpotLights()
{
    return m_maxSpotLights;
}

std::string Scene::GetFileName(const std::string& _filePath)
{
    std::filesystem::path path(_filePath);
    return path.stem().string();
}

std::vector<GameObject*>& Scene::GetGameObjects()
{
    return gameObjects;
}

GameObject* Scene::GetWorld()
{
    return m_world;
}

Guid Scene::GetGuid()
{
    return  m_guid;
}

void Scene::SetGuid(Guid _guid)
{
    m_guid = _guid;
}


