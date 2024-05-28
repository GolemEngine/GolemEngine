#pragma once

#include "dll.h"

#include <vector>
#include <string>
#include <unordered_map>


#include "Utils/tools.h"

class Skybox
{
private:
	Skybox();
	Skybox(const Skybox& skybox) = delete;
	~Skybox();

	Skybox* m_instance;

	static unsigned int m_cubemapId;
	static unsigned int m_skyboxVAO;
	static unsigned int m_skyboxVBO;

	std::unordered_map<std::string, unsigned int> m_loadedTextures;

public:
	GOLEM_ENGINE_API static Skybox& GetInstance();
	GOLEM_ENGINE_API unsigned int LoadCubemap(std::vector<std::string> faces);
	GOLEM_ENGINE_API void SetTexture();
	GOLEM_ENGINE_API void SetSpaceSkybox();
	GOLEM_ENGINE_API unsigned int GetSkyboxCubeMapId();
	GOLEM_ENGINE_API unsigned int GetSkyboxVAO();

	std::vector<std::string> faces
	{
		Tools::FindFile("skybox_right.jpg"),
		Tools::FindFile("skybox_left.jpg"),
		Tools::FindFile("skybox_bottom.jpg"),
		Tools::FindFile("skybox_top.jpg"),
		Tools::FindFile("skybox_front.jpg"),
		Tools::FindFile("skybox_back.jpg"),
	};

	std::vector<std::string> faces2
	{
		Tools::FindFile("Left_Tex.jpg"),
		Tools::FindFile("Right_Tex.jpg"),
		Tools::FindFile("Down_Tex.jpg"),
		Tools::FindFile("Up_Tex.jpg"),
		Tools::FindFile("Front_Tex.jpg"),
		Tools::FindFile("Back_Tex.jpg"),
	};
};