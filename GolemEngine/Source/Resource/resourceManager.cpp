#include "Resource/resourceManager.h"
#include "Utils/tools.h"
#include "Resource/Rendering/model.h"
#include "Resource/Rendering/texture.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Resource/Rendering/Shader/computeShader.h"
#include "Resource/Rendering/Shader/vertexShader.h"
#include "Resource/Rendering/Shader/fragmentShader.h"
#include "Resource/Rendering/skybox.h"
#include "Utils/viewportTools.h"

ResourceManager* ResourceManager::m_instancePtr = nullptr;

void ResourceManager::ProcessFile(const std::filesystem::path& _filePath)
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    std::string fileName = _filePath.filename().string();
    std::string extension = _filePath.extension().string();
    std::string filePath = _filePath.string();

    if (extension == ".obj")
    {
        Model* model = resourceManager->Create<Model>(fileName, filePath);
        model->Load(filePath.c_str());
    }
    else if (extension == ".jpg" || extension == ".png")
    {
        Texture* texture = resourceManager->Create<Texture>(fileName, filePath);
        texture->Load(filePath.c_str());
    }
}

void ResourceManager::TraverseDirectoryAndLoadFiles(const std::filesystem::path& _directoryPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(_directoryPath))
    {
        if (entry.is_regular_file())
        {
            ProcessFile(entry.path());
        }
        else if (entry.is_directory())
        {
            TraverseDirectoryAndLoadFiles(entry.path());
        }
    }
}

ResourceManager* ResourceManager::GetInstance()
{
    if (!m_instancePtr) 
    {
        m_instancePtr = new ResourceManager();
    }
    return m_instancePtr;
}

void ResourceManager::CreateAndLoadResources()
{
    ResourceManager* resourceManager = ResourceManager::GetInstance();

    resourceManager->TraverseDirectoryAndLoadFiles(Tools::FindFolder("Assets"));

    // Set default shader for general stuff
    Shader* defaultShader = resourceManager->Create<Shader>(m_defaultShader);
    VertexShader* defaultVertexShader = new VertexShader(defaultShader, Tools::FindFile("default.vs").c_str());
    FragmentShader* defaultFragmentShader = new FragmentShader(defaultShader, Tools::FindFile("default.fs").c_str());
    defaultShader->SetAllShaders(defaultVertexShader, defaultFragmentShader);

#pragma region Terrain Shaders
    Shader* terrainShader = resourceManager->Create<Shader>(m_terrainShader);
    VertexShader* terrainVertexShader = new VertexShader(terrainShader, Tools::FindFile("terrain.vs").c_str());
    FragmentShader* terrainFragmentShader = new FragmentShader(terrainShader, Tools::FindFile("terrain.fs").c_str());
    terrainShader->SetAllShaders(terrainVertexShader, terrainFragmentShader);

    Shader* terrainUvShader = resourceManager->Create<Shader>(m_terrainUvShader);
    VertexShader* terrainVertexUvShader = new VertexShader(terrainUvShader, Tools::FindFile("terrainUv.vs").c_str());
    FragmentShader* terrainFragmentUvShader = new FragmentShader(terrainUvShader, Tools::FindFile("terrainUv.fs").c_str());
    terrainUvShader->SetAllShaders(terrainVertexUvShader, terrainFragmentUvShader);
    
    ComputeShader* defaultTerrainComputeShader = resourceManager->Create<ComputeShader>(m_terrainComputeShader);
    defaultTerrainComputeShader->SetComputePath(Tools::FindFile("terrain.comp"));
    defaultTerrainComputeShader->Init();
#pragma endregion Terrain Shaders

    Shader* ColliderShader = resourceManager->Create<Shader>(m_colliderShader);
    VertexShader* colliderVertexShader = new VertexShader(ColliderShader, Tools::FindFile("collider.vs").c_str());
    FragmentShader* colliderFragmentShader = new FragmentShader(ColliderShader, Tools::FindFile("collider.fs").c_str());
    ColliderShader->SetAllShaders(colliderVertexShader, colliderFragmentShader);

    Shader* skyboxShader = resourceManager->Create<Shader>(m_skyboxShader);
    VertexShader* skyboxVertexShader = new VertexShader(skyboxShader, Tools::FindFile("skybox.vs").c_str());
    FragmentShader* skyboxFragmentShader = new FragmentShader(skyboxShader, Tools::FindFile("skybox.fs").c_str());

    skyboxShader->SetAllShaders(skyboxVertexShader, skyboxFragmentShader);
    Skybox::GetInstance().SetTexture();
    skyboxShader->Use();
    skyboxShader->GetFragmentShader()->SetInt("skybox", 0);

    
    Texture* defaultTexture = resourceManager->Create<Texture>(m_defaultTexture, Tools::FindFile("default_texture.png"));
    defaultTexture->Load(defaultTexture->path.c_str());
    
    Texture* defaultGridTerrainTexture = resourceManager->Create<Texture>(m_defaultGridTerrainTexture, Tools::FindFile("grid_2.png"));
    defaultGridTerrainTexture->Load(defaultGridTerrainTexture->path.c_str());
    
    Texture* vikingTexture = resourceManager->Create<Texture>("viking_texture", Tools::FindFile("viking_room.jpg"));
    vikingTexture->Load(vikingTexture->path.c_str());
    
    Texture* allBaldTexture = resourceManager->Create<Texture>("all_bald_texture", Tools::FindFile("all_bald.jpg"));
    allBaldTexture->Load(allBaldTexture->path.c_str());
    
    Model* vikingModel = resourceManager->Create<Model>("viking_room", Tools::FindFile("viking_room.obj"));
    vikingModel->Load(vikingModel->path.c_str());
    
    Model* defaultModel = resourceManager->Create<Model>(m_defaultModel, Tools::FindFile("cube.obj"));
    defaultModel->Load(defaultModel->path.c_str());
    
    // Needed to avoid same resource reference problem for colliders
    Model* sphere = resourceManager->Create<Model>("sphereCollider.obj", Tools::FindFile("sphere.obj"));
    sphere->Load(sphere->path.c_str());
    
    Model* cube = resourceManager->Create<Model>("cubeCollider.obj", Tools::FindFile("cube.obj"));
    cube->Load(cube->path.c_str());

    Model* capsule = resourceManager->Create<Model>("capsuleCollider.obj", Tools::FindFile("capsule.obj"));
    capsule->Load(capsule->path.c_str());

    Texture* dirtTexture = resourceManager->Create<Texture>("dirt_texture", Tools::FindFile("dirt.png"));
    dirtTexture->Load(dirtTexture->path.c_str());

    Texture* grassTexture = resourceManager->Create<Texture>("grass_texture", Tools::FindFile("grass.jpg"));
    grassTexture->Load(grassTexture->path.c_str());

    Texture* snowTexture = resourceManager->Create<Texture>("snow_texture", Tools::FindFile("snow.jpg"));
    snowTexture->Load(snowTexture->path.c_str());

    // Skybox Init
    //Shader* skyboxShader = resourceManager->Create<Shader>("skybox_shader", Tools::FindFile("skybox.vs"));
    //skyboxShader->SetVertexAndFragmentShader(skyboxShader->path.c_str(), Tools::FindFile("skybox.fs").c_str());
    //Skybox::GetInstance().SetTexture();
    //skyboxShader->Use();
    //skyboxShader->SetInt("skybox", 0);
}

void ResourceManager::DeleteAllResouces()
{
    for (auto& resource : m_resources)
    {
        delete resource.second;
    }
}


std::string ResourceManager::GetDefaultShader()
{
    return m_defaultShader;
}

std::string ResourceManager::GetDefaultTexture()
{
    return m_defaultTexture;
}

std::string ResourceManager::GetDefaultModel()
{
    return m_defaultModel;
}

std::string ResourceManager::GetTerrainShader()
{
    return m_terrainShader;
}

std::string ResourceManager::GetTerrainUvShader()
{
    return m_terrainUvShader;
}

std::string ResourceManager::GetTerrainComputeShader()
{
    return m_terrainComputeShader;
}

std::string ResourceManager::GetGridTerrainTexture()
{
    return m_defaultGridTerrainTexture;
}
std::string ResourceManager::GetSkyboxShader()
{
    return m_skyboxShader;
}

std::string ResourceManager::GetColliderShader()
{
    return m_colliderShader;
}
std::string ResourceManager::GetDirtTexture()
{
    return m_dirtTexture;
}

std::string ResourceManager::GetGrassTexture()
{
    return m_grassTexture;
}

std::string ResourceManager::GetSnowTexture()
{
    return m_snowTexture;
}
