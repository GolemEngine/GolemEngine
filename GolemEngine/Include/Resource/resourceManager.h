
#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>

#include "dll.h"
#include "Resource.h"
#include "Debug/log.h"


class ResourceManager
{
private:
	// Static pointer which points to the only instance of this class
	static ResourceManager* m_instancePtr;

	static inline std::string m_defaultShader = "default_shader";
	static inline std::string m_defaultTexture = "default_texture";
	static inline std::string m_defaultModel = "default_model";
	static inline std::string m_terrainShader = "default_terrain_shader";    // For black and white gradient
	static inline std::string m_terrainUvShader = "default_terrain_uv_shader";    // For selecting terrain
	static inline std::string m_terrainComputeShader = "default_terrain_compute_shader";
	static inline std::string m_defaultGridTerrainTexture = "default_grid_terrain_texture";
	static inline std::string m_colliderShader = "collider_shader";
	static inline std::string m_skyboxShader = "skybox_shader";

	static inline std::string m_dirtTexture = "dirt_texture";
	static inline std::string m_grassTexture = "grass_texture";
	static inline std::string m_snowTexture = "snow_texture";

	static inline std::unordered_map<std::string, Resource*> m_resources;
private:
	ResourceManager() {}

public:
	GOLEM_ENGINE_API static ResourceManager* GetInstance();
	GOLEM_ENGINE_API static void CreateAndLoadResources();
	GOLEM_ENGINE_API static void DeleteAllResouces();

	GOLEM_ENGINE_API static std::string GetDefaultShader();
	GOLEM_ENGINE_API static std::string GetDefaultTexture();
	GOLEM_ENGINE_API static std::string GetDefaultModel();
	GOLEM_ENGINE_API static std::string GetTerrainShader();
	GOLEM_ENGINE_API static std::string GetTerrainUvShader();
	GOLEM_ENGINE_API static std::string GetTerrainComputeShader();
	GOLEM_ENGINE_API static std::string GetGridTerrainTexture();
	GOLEM_ENGINE_API static std::string GetSkyboxShader();
	GOLEM_ENGINE_API static std::string GetColliderShader();
	GOLEM_ENGINE_API static std::string GetDirtTexture();
	GOLEM_ENGINE_API static std::string GetGrassTexture();
	GOLEM_ENGINE_API static std::string GetSnowTexture();
	GOLEM_ENGINE_API void ProcessFile(const std::filesystem::path& _filePath);
	GOLEM_ENGINE_API void TraverseDirectoryAndLoadFiles(const std::filesystem::path& _directoryPath);

	template<class T>
	T* Create(std::string _name, std::string _path);
	template<class T>
	T* Create(std::string _name);
	template<class T>
	T* Get(std::string _name);
	void Delete(std::string _name);

	GOLEM_ENGINE_API std::unordered_map<std::string, Resource*> GetResources();
};

template<class T>
inline T* ResourceManager::Create(std::string _name, std::string _path)
{
	if (m_resources[_name] != nullptr)
	{
		Delete(_name);
	}
	T* resource = new T;
	m_resources[_name] = resource;
	m_resources[_name]->path = _path;
	return resource;
}

template<class T>
inline T* ResourceManager::Create(std::string _name)
{
	if (m_resources[_name] != nullptr)
	{
		Delete(_name);
	}
	T* resource = new T;
	m_resources[_name] = resource;
	return resource;
}

template<class T>
inline T* ResourceManager::Get(std::string _name)
{
	if (!m_resources[_name])
	{
		Log::Print("No resource of name '%s' exists", _name.c_str());
		return nullptr;
	}
	return dynamic_cast<T*>(m_resources[_name]);
}

inline void ResourceManager::Delete(std::string _name)
{
	delete m_resources[_name];
	m_resources.erase(_name);
}

inline std::unordered_map<std::string, Resource*> ResourceManager::GetResources()
{
	return m_resources;
}
