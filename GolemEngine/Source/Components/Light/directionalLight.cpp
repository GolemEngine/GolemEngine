#include "..\..\..\Include\Components\Light\directionalLight.h"

#include "Debug/log.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Resource/sceneManager.h"
#include "Resource/Rendering/Shader/vertexShader.h"

DirectionalLight::DirectionalLight()
	: Light(Vector4(1.f, 1.f, 1.f, 1.f), Vector4(1.f, 1.f, 1.f, 1.f), Vector4(1.f, 1.f, 1.f, 1.f))
{
	const std::vector<DirectionalLight*>& dirLights = SceneManager::GetCurrentScene()->GetDirectionalLights();
	size_t maxDirs = SceneManager::GetCurrentScene()->GetMaxDirectionalLights();

	direction = Vector3(0.f, 0.f, 0.f);

	id = dirLights.size();
	if (id >= maxDirs)
	{
		Log::Print("The DirectionalLight %d will not be used. DirectionalLights limit : %d", id, maxDirs);
	}
	else
	{
		SceneManager::GetCurrentScene()->AddLight(this);
	}
};

DirectionalLight::DirectionalLight(const Vector4& _diffuse, const Vector4& _ambient, const Vector4& _specular, const Vector3& _direction)
	: Light(_diffuse, _ambient, _specular), direction(_direction)
{
	const std::vector<DirectionalLight*>& dirLights = SceneManager::GetCurrentScene()->GetDirectionalLights();
	size_t maxDirs = SceneManager::GetCurrentScene()->GetMaxDirectionalLights();

	id = dirLights.size();
	if (id >= maxDirs)
	{
		Log::Print("The DirectionalLight %d will not be used. DirectionalLights limit : %d", id, maxDirs);
	}
	else
	{
		SceneManager::GetCurrentScene()->AddLight(this);
	}
};

DirectionalLight::~DirectionalLight()
{
	SceneManager::GetCurrentScene()->DeleteLight(this);
}

void DirectionalLight::SetDirectionalLight(Shader* _shader)
{
	_shader->GetFragmentShader()->SetVec3("dirLights[" + std::to_string(id) + "].direction", direction);
	_shader->GetFragmentShader()->SetVec4("dirLights[" + std::to_string(id) + "].ambient", ambientColor);
	_shader->GetFragmentShader()->SetVec4("dirLights[" + std::to_string(id) + "].diffuse", diffuseColor);
	_shader->GetFragmentShader()->SetVec4("dirLights[" + std::to_string(id) + "].specular", specularColor);
}

void DirectionalLight::Update()
{
}
