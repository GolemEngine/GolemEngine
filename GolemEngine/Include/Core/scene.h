#pragma once

#include <nlohmann/json.hpp>

#include "dll.h"
#include "gameobject.h"
#include "Resource/resourceManager.h"
#include "Resource/guid.h"
#include "Core/camera.h"
#include "Debug/log.h"
#include "Components/Light/light.h"
#include "WorldBuilder/terrain.h"
#include "Components/Physic/collider.h"

using json = nlohmann::json;

class DirectionalLight;
class PointLight;
class SpotLight;
class Shader;
class Mesh;

class Scene
{
private:
	Guid m_guid;
	GameObject* m_world = nullptr;

	static constexpr size_t m_maxDirLights = 3;
	static constexpr size_t m_maxPointLights = 10;    // According to the shader
	static constexpr size_t m_maxSpotLights = 10;

	std::vector<DirectionalLight*> m_dirLights;
	std::vector<PointLight*> m_pointLights;
	std::vector<SpotLight*> m_spotLights;
	std::vector<Mesh*> m_meshes;
	std::vector<GameObject*> m_deletedGameObjects;

public:
	std::string name;
	std::vector<GameObject*> gameObjects;

	// std::vector<Terrain*> terrains;
	bool isNewObjectDropped = false;
	std::string loadingObject;
	
public:
	// Create a scene by giving it a name and setting _isEmpty to 0 or 1. 0 means the scene will be a default
	// scene with a few objects to start, 1 means the scene will have nothing in it (useful for creating scenes from files)
	GOLEM_ENGINE_API Scene(std::string _name, bool _makeSceneEmpty);

	GOLEM_ENGINE_API void Test(Collider* _collider, Collider* _other);
	GOLEM_ENGINE_API void InitDefaultScene();
	GOLEM_ENGINE_API void InitLights();
	GOLEM_ENGINE_API void Update(Camera* _camera);
	GOLEM_ENGINE_API void UpdateGameObjects(Camera* _camera);
	GOLEM_ENGINE_API void UpdateLights(Shader* _shader);
	// Check if the gameobject's name is already in the vector or not.
	GOLEM_ENGINE_API bool IsNameExists(const std::string& _name);
	GOLEM_ENGINE_API void CreateNewObject(std::string _name, std::string _modelName, std::string _textureName = "", std::string _shaderName = "");
	GOLEM_ENGINE_API void CreateNewModel(std::string _filePath, std::string _resourceName = "");
	GOLEM_ENGINE_API void AddLight(Light* _light);
	
	GOLEM_ENGINE_API const std::vector<DirectionalLight*>& GetDirectionalLights();
	GOLEM_ENGINE_API const std::vector<PointLight*>& GetPointLights();
	GOLEM_ENGINE_API const std::vector<SpotLight*>& GetSpotLights();
	GOLEM_ENGINE_API size_t GetMaxDirectionalLights();
	GOLEM_ENGINE_API size_t GetMaxPointLights();
	GOLEM_ENGINE_API size_t GetMaxSpotLights();
	GOLEM_ENGINE_API std::string GetFileName(const std::string& _filePath);

	GOLEM_ENGINE_API std::vector<GameObject*>& GetGameObjects();

	GOLEM_ENGINE_API GameObject* GetWorld();
	GOLEM_ENGINE_API Guid GetGuid();

	GOLEM_ENGINE_API void AddGameObject(GameObject* _gameObject);
	GOLEM_ENGINE_API void AddDeletedGameObject(GameObject* _gameObject);
	GOLEM_ENGINE_API void RemoveGameObject(GameObject* _gameObject);
	GOLEM_ENGINE_API void DeleteLight(Light* _light);
	GOLEM_ENGINE_API void SetGuid(Guid _guid);


	// Define serialization and deserialization functions manually because the
	// macro is not used due to the pointer member variable.
	GOLEM_ENGINE_API void ToJson(json& _j) const
	{
		_j = json
		{
			{"name", name},
			{"guid", m_guid.ToString()},
		};
		if (!gameObjects.empty())
		{
			json jGameObjects;
			for (int i = 0; i < gameObjects.size(); i++)
			{
				json jGameObjectPtr;
				gameObjects[i]->ToJson(jGameObjectPtr);
				jGameObjects.push_back(jGameObjectPtr);
			}
			_j["gameObjects"] = jGameObjects;
		}
	}
};