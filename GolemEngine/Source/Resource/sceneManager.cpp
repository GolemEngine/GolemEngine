#include "Resource/sceneManager.h"

#include <fstream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Components/audio.h"
#include "Components/meshRenderer.h"
#include "Components/Light/directionalLight.h"
#include "Components/Light/pointLight.h"
#include "Components/Light/spotLight.h"
#include "Utils/tools.h"
#include "Wrappers/graphicWrapper.h"
#include "Wrappers/windowWrapper.h"
#include "Core/mesh.h"
#include "Resource/Rendering/model.h"
#include "Resource/Rendering/texture.h"
#include "Utils/tools.h"
#include "Resource/resourceManager.h"
#include "WorldBuilder/terrain.h"
#include "WorldBuilder/defaultTerrain.h"
#include "WorldBuilder/noisemapTerrain.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui.h"

using json = nlohmann::json;


void SceneManager::Init()
{
    // Create a framebuffer and pass the scene in it to be used in the viewport 
    GraphicWrapper::CreateFramebuffer(1636, 980);

    if (Tools::GetFolderSize(Tools::FindFolder("Scenes")) != 0)    // Check if there are already saved scenes
    {
        // Init scenes that are already saved
        std::vector<std::string> sceneNames = Tools::GetFolderElementsNames(Tools::FindFolder("Scenes"));
        for (int i = 0; i < Tools::GetFolderSize(Tools::FindFolder("Scenes")); i++)
        {
            CreateSceneFromFile(Tools::RemoveExtension(sceneNames[i]));
        }
        // Load the first scene
        LoadScene(0);
    }
    else    // If there are no scenes saved, Create one
    {
        CreateScene("scene_0");
        SaveScene();
    }
}

void SceneManager::SaveScene()
{
    json jScene;
    // Get scene name and add .json type to it
    GetCurrentScene()->ToJson(jScene);
    std::string sceneFileName = GetCurrentScene()->name;
    sceneFileName.append(".json");

    // Create or find the file and save it
    std::fstream sceneFile;
    if (Tools::FindFile(sceneFileName) != "")
    {
        sceneFile.open(Tools::FindFile(sceneFileName), std::ios::out);
        sceneFile << jScene.dump(2);
    }
    else if (Tools::FindFile(sceneFileName) == "")
    {
        sceneFile.open(Tools::FindFolder("Scenes").append("\\" + sceneFileName), std::ios::out);
        sceneFile << jScene.dump(2);
    }
}

void SceneManager::LoadScene(int _id)
{
    m_currentScene = m_scenes[_id];
}

void SceneManager::CreateScene(std::string _sceneName)
{
    m_scenes.push_back(new Scene(_sceneName, 0));
}

void SceneManager::CreateSceneFromFile(std::string _sceneName)
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();
    // Get the name pf the scene and append .json
    std::string sceneFileName = _sceneName;
    sceneFileName.append(".json");
    // Open the corresponding folder
    std::fstream sceneFile;
    sceneFile.open(Tools::FindFile(sceneFileName), std::ios::in);
    // Parse the json doc to a json variable here
    json jScene{json::parse(sceneFile)};
    
    Scene* scene = new Scene(_sceneName, 1);
    scene->name = jScene["name"];

    for (int i = 0; i < jScene["gameObjects"].size(); i++)
    {
        SetupWorldFromJson(scene, jScene, i);
        SetupGameObjectsFromJson(scene, jScene, i);
        SetupDefaultTerrainsFromJson(scene, jScene, i);
        SetupNoisemapTerrainsFromJson(scene, jScene, i);
    }
    
    m_scenes.push_back(scene);    
}

void SceneManager::SetupWorldFromJson(Scene* _scene, json _jScene, int _i)
{
    // Check if the gameobject is the world
    if (_jScene["gameObjects"][_i]["name"] != "World")
    {
        return;
    }

    // Setup guid
    Guid worldGuid;
    worldGuid.FromString(_jScene["gameObjects"][_i]["guid"]);
    _scene->GetWorld()->guid = worldGuid;

    // Setup components
    for (int j = 0; j < _jScene["gameObjects"][j]["components"].size(); j++)
    {
        // Setup transform component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "transform")
        {
            Guid transformGuid;
            transformGuid.FromString(_jScene["gameObjects"][_i]["components"][0]["data"]["guid"]);
            _scene->GetWorld()->transform->guid = transformGuid;
            _scene->GetWorld()->transform->localPosition = _jScene["gameObjects"][_i]["components"][0]["data"]["localPosition"];
            _scene->GetWorld()->transform->rotation = _jScene["gameObjects"][_i]["components"][0]["data"]["rotation"];
            _scene->GetWorld()->transform->scaling = _jScene["gameObjects"][_i]["components"][0]["data"]["scaling"];

        }
        // Setup directional light
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "directionalLight")
        {
            _scene->GetWorld()->GetComponent<DirectionalLight>()->id = _jScene["gameObjects"][_i]["components"][j]["data"]["id"];
            _scene->GetWorld()->GetComponent<DirectionalLight>()->diffuseColor = _jScene["gameObjects"][_i]["components"][j]["data"]["diffuseColor"];
            _scene->GetWorld()->GetComponent<DirectionalLight>()->ambientColor = _jScene["gameObjects"][_i]["components"][j]["data"]["ambientColor"];
            _scene->GetWorld()->GetComponent<DirectionalLight>()->specularColor = _jScene["gameObjects"][_i]["components"][j]["data"]["specularColor"];
            _scene->GetWorld()->GetComponent<DirectionalLight>()->direction = _jScene["gameObjects"][_i]["components"][j]["data"]["direction"];
        }
    }
}

void SceneManager::SetupGameObjectsFromJson(Scene* _scene, json _jScene, int _i)
{
    // Check if the GameObject isn't a terrain or world. Null condition is added because of a bug discovered
    if (_jScene["gameObjects"][_i]["name"].is_null() ||
        _jScene["gameObjects"][_i]["name"] == "World" ||
        _jScene["gameObjects"][_i]["isTerrain"] == true)
    {
        return;
    }

    // Setup name
    GameObject* gameObject = new GameObject();
    gameObject->name = _jScene["gameObjects"][_i]["name"];
    // Setup guid
    Guid gameObjectGuid;
    gameObjectGuid.FromString(_jScene["gameObjects"][_i]["guid"]);
    gameObject->guid = gameObjectGuid;

    // Setup components
    for (int j = 0; j < _jScene["gameObjects"][_i]["components"].size(); j++)
    {
        // Setup transform component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "transform")
        {
            Guid transformGuid;
            transformGuid.FromString(_jScene["gameObjects"][_i]["components"][j]["data"]["guid"]);
            gameObject->transform->guid = transformGuid;
            gameObject->transform->localPosition = _jScene["gameObjects"][_i]["components"][j]["data"]["localPosition"];
            gameObject->transform->rotation = _jScene["gameObjects"][_i]["components"][j]["data"]["rotation"];
            gameObject->transform->scaling = _jScene["gameObjects"][_i]["components"][j]["data"]["scaling"];
        }
        // Setup directionalLight component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "directionalLight")
        {
            gameObject->AddComponent<DirectionalLight>();
            gameObject->GetComponent<DirectionalLight>()->id = _jScene["gameObjects"][_i]["components"][j]["data"]["id"];
            gameObject->GetComponent<DirectionalLight>()->diffuseColor = _jScene["gameObjects"][_i]["components"][j]["data"]["diffuseColor"];
            gameObject->GetComponent<DirectionalLight>()->ambientColor = _jScene["gameObjects"][_i]["components"][j]["data"]["ambientColor"];
            gameObject->GetComponent<DirectionalLight>()->specularColor = _jScene["gameObjects"][_i]["components"][j]["data"]["specularColor"];
            gameObject->GetComponent<DirectionalLight>()->direction = _jScene["gameObjects"][_i]["components"][j]["data"]["direction"];
        }
        // Setup pointLight component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "pointLight")
        {
            gameObject->AddComponent<PointLight>();
            gameObject->GetComponent<PointLight>()->id = _jScene["gameObjects"][_i]["components"][j]["data"]["id"];
            gameObject->GetComponent<PointLight>()->diffuseColor = _jScene["gameObjects"][_i]["components"][j]["data"]["diffuseColor"];
            gameObject->GetComponent<PointLight>()->ambientColor = _jScene["gameObjects"][_i]["components"][j]["data"]["ambientColor"];
            gameObject->GetComponent<PointLight>()->specularColor = _jScene["gameObjects"][_i]["components"][j]["data"]["specularColor"];
            gameObject->GetComponent<PointLight>()->position = _jScene["gameObjects"][_i]["components"][j]["data"]["position"];
            gameObject->GetComponent<PointLight>()->constant = _jScene["gameObjects"][_i]["components"][j]["data"]["constant"];
            gameObject->GetComponent<PointLight>()->linear = _jScene["gameObjects"][_i]["components"][j]["data"]["linear"];
            gameObject->GetComponent<PointLight>()->quadratic = _jScene["gameObjects"][_i]["components"][j]["data"]["quadratic"];
        }
        // Setup spotLight component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "spotLight")
        {
            gameObject->AddComponent<SpotLight>();
            gameObject->GetComponent<SpotLight>()->id = _jScene["gameObjects"][_i]["components"][j]["data"]["id"];
            gameObject->GetComponent<SpotLight>()->diffuseColor = _jScene["gameObjects"][_i]["components"][j]["data"]["diffuseColor"];
            gameObject->GetComponent<SpotLight>()->ambientColor = _jScene["gameObjects"][_i]["components"][j]["data"]["ambientColor"];
            gameObject->GetComponent<SpotLight>()->specularColor = _jScene["gameObjects"][_i]["components"][j]["data"]["specularColor"];
            gameObject->GetComponent<SpotLight>()->position = _jScene["gameObjects"][_i]["components"][j]["data"]["position"];
            gameObject->GetComponent<SpotLight>()->direction = _jScene["gameObjects"][_i]["components"][j]["data"]["direction"];
            gameObject->GetComponent<SpotLight>()->constant = _jScene["gameObjects"][_i]["components"][j]["data"]["constant"];
            gameObject->GetComponent<SpotLight>()->linear = _jScene["gameObjects"][_i]["components"][j]["data"]["linear"];
            gameObject->GetComponent<SpotLight>()->quadratic = _jScene["gameObjects"][_i]["components"][j]["data"]["quadratic"];
            gameObject->GetComponent<SpotLight>()->cutOff = _jScene["gameObjects"][_i]["components"][j]["data"]["cutOff"];
            gameObject->GetComponent<SpotLight>()->outerCutOff = _jScene["gameObjects"][_i]["components"][j]["data"]["outerCutOff"];
        }
        // Setup audio component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "audio")
        {
            gameObject->AddComponent<Audio>();
            gameObject->GetComponent<Audio>()->musicPath = Tools::GetPathFromJsonString(_jScene["gameObjects"][_i]["components"][j]["data"]["musicPath"]);
            gameObject->GetComponent<Audio>()->SetVolume(_jScene["gameObjects"][_i]["components"][j]["data"]["volume"]);
            gameObject->GetComponent<Audio>()->SetLoop(_jScene["gameObjects"][_i]["components"][j]["data"]["isLooping"]);
            gameObject->GetComponent<Audio>()->StopMusic(_jScene["gameObjects"][_i]["components"][j]["data"]["isPlaying"]);
        }
        // Setup meshRenderer component
        if (_jScene["gameObjects"][_i]["components"][j]["name"] == "meshRenderer")
        {
            // TODO : load specifique model with correct shader texture and model
            ResourceManager* resourceManager = ResourceManager::GetInstance();

            Texture* texture = resourceManager->Get<Texture>(Tools::GetFileNameFromPath(_jScene["gameObjects"][_i]["components"][j]["data"]["texturePath"]));
            Model* model = resourceManager->Get<Model>(Tools::GetFileNameFromPath(_jScene["gameObjects"][_i]["components"][j]["data"]["modelPath"]));
            Mesh* mesh = new Mesh(model, texture, resourceManager->Get<Shader>(ResourceManager::GetDefaultShader()));
            gameObject->AddComponent(new MeshRenderer(mesh));
        }
    }
}

void SceneManager::SetupDefaultTerrainsFromJson(Scene* _scene, json _jScene, int _i)
{
    // Check if the GameObject isn't world. Null condition is added because of a bug discovered
    if (_jScene["gameObjects"][_i]["name"].is_null() ||
        _jScene["gameObjects"][_i]["name"] == "World" ||
        _jScene["gameObjects"][_i]["isTerrain"] == false)
    {
        return;
    }
    // Check if the terrain is a default terraain
    if (_jScene["gameObjects"][_i]["terrainData"]["noisemapPath"] != "")
    {
        return;
    }

    // Setup name
    std::string name = _jScene["gameObjects"][_i]["name"];
    Transform* transform = new Transform();    // Set transform later
    DefaultTerrain* defaultTerrain = new DefaultTerrain(name, transform);

    // Setup components
    for (int i = 0; i < _jScene["gameObjects"][_i]["components"].size(); i++)
    {
        // Setup transform
        if (_jScene["gameObjects"][_i]["components"][i]["name"] == "transform")
        {
            Guid transformGuid;
            transformGuid.FromString(_jScene["gameObjects"][_i]["components"][i]["data"]["guid"]);
            defaultTerrain->transform->guid = transformGuid;
            defaultTerrain->transform->localPosition = _jScene["gameObjects"][_i]["components"][i]["data"]["localPosition"];
            defaultTerrain->transform->rotation = _jScene["gameObjects"][_i]["components"][i]["data"]["rotation"];
            defaultTerrain->transform->scaling = _jScene["gameObjects"][_i]["components"][i]["data"]["scaling"];
        }
    }

    defaultTerrain->Init(_jScene["gameObjects"][_i]["terrainData"]["xResolution"], _jScene["gameObjects"][_i]["terrainData"]["zResolution"]);
}

void SceneManager::SetupNoisemapTerrainsFromJson(Scene* _scene, json _jScene, int _i)
{
    // Check if the GameObject isn't world. Null condition is added because of a bug discovered
    if (_jScene["gameObjects"][_i]["name"].is_null() ||
        _jScene["gameObjects"][_i]["name"] == "World" ||
        _jScene["gameObjects"][_i]["isTerrain"] == false)
    {
        return;
    }
    // Check if the terrain is a default terraain
    if (_jScene["gameObjects"][_i]["terrainData"]["noisemapPath"] == "")
    {
        return;
    }

    // Setup name
    std::string name = _jScene["gameObjects"][_i]["name"];
    Transform* transform = new Transform();    // Set transform later
    NoisemapTerrain* noisemapTerrain = new NoisemapTerrain(name, transform);

    // Setup components
    for (int i = 0; i < _jScene["gameObjects"][_i]["components"].size(); i++)
    {
        // Setup transform
        if (_jScene["gameObjects"][_i]["components"][i]["name"] == "transform")
        {
            Guid transformGuid;
            transformGuid.FromString(_jScene["gameObjects"][_i]["components"][i]["data"]["guid"]);
            noisemapTerrain->transform->guid = transformGuid;
            noisemapTerrain->transform->localPosition = _jScene["gameObjects"][_i]["components"][i]["data"]["localPosition"];
            noisemapTerrain->transform->rotation = _jScene["gameObjects"][_i]["components"][i]["data"]["rotation"];
            noisemapTerrain->transform->scaling = _jScene["gameObjects"][_i]["components"][i]["data"]["scaling"];
        }
    }

    std::string noisemapPath = Tools::GetPathFromJsonString(_jScene["gameObjects"][_i]["terrainData"]["noisemapPath"]);
    noisemapTerrain->Init(noisemapPath.c_str());
}

Scene* SceneManager::GetCurrentScene()
{
    return m_currentScene;
}

Scene* SceneManager::GetScene(int _id)
{
    Scene* scene = nullptr;
    for (int i = 0; i < m_scenes.size(); i++)
    {
        if (i == _id)
        {
            scene = m_scenes[i];
        }
    }
    
    return  scene;
}

size_t SceneManager::GetSceneCount()
{
    return  m_scenes.size();
}

void SceneManager::SetCurrentScene(Scene* _scene)
{
    m_currentScene = _scene;
}