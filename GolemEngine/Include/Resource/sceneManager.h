#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>


#include "dll.h"
#include "Core/scene.h"

using json = nlohmann::json;


class SceneManager
{
private:
    static inline std::vector<Scene*> m_scenes;
    static inline Scene* m_currentScene = nullptr;
    
private:
    // Private constructors and destructor to make static class
    SceneManager() = delete;
    SceneManager(SceneManager& _other) = delete;
    SceneManager(SceneManager&& _other) = delete;
    SceneManager& operator=(SceneManager& _other) = delete;
    SceneManager& operator=(SceneManager&& _other) = delete;
    void operator=(const SceneManager&) = delete;
    ~SceneManager() = delete;

public:
    GOLEM_ENGINE_API static void Init();

    GOLEM_ENGINE_API static void SaveScene();
    GOLEM_ENGINE_API static void LoadScene(int _id);
    GOLEM_ENGINE_API static void CreateScene(std::string _sceneName);
    GOLEM_ENGINE_API static void CreateSceneFromFile(std::string _sceneFileName);
    GOLEM_ENGINE_API static void SetupWorldFromJson(Scene* _scene, json _jScene, int _i);
    GOLEM_ENGINE_API static void SetupGameObjectsFromJson(Scene* _scene, json _jScene, int _i);
    GOLEM_ENGINE_API static void SetupDefaultTerrainsFromJson(Scene* _scene, json _jScene, int _i);
    GOLEM_ENGINE_API static void SetupNoisemapTerrainsFromJson(Scene* _scene, json _jScene, int _i);

    GOLEM_ENGINE_API static Scene* GetCurrentScene();
    GOLEM_ENGINE_API static Scene* GetScene(int _id);
    GOLEM_ENGINE_API static size_t GetSceneCount();

    GOLEM_ENGINE_API static void SetCurrentScene(Scene* _scene);
};