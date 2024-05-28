#include "Components\Light\pointLight.h"

#include "Debug/log.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Core/gameObject.h"
#include "matrix4.h"
#include "Resource/sceneManager.h"
#include "Resource/Rendering/Shader/vertexShader.h"

PointLight::PointLight()
    : Light(Vector4(1.f, 1.f, 1.f, 1.f), Vector4(1.f, 1.f, 1.f, 1.f), Vector4(1.f, 1.f, 1.f, 1.f))
{
    std::vector<PointLight*> pointLights = SceneManager::GetCurrentScene()->GetPointLights();
    size_t maxPoints = SceneManager::GetCurrentScene()->GetMaxPointLights();

    constant = 1.f;
    linear = 0.f;
    quadratic = 0.f;

    id = pointLights.size();
    if (id >= maxPoints)
    {
        Log::Print("The PointLight %d will not be used. PointLights limit : %d", id, maxPoints);
    }
    else
    {
        SceneManager::GetCurrentScene()->AddLight(this);
    }
}

PointLight::PointLight(const Vector4& _diffuse, const Vector4& _ambient, const Vector4& _specular, const Vector3& _position, 
    const float _constant, const float _linear, const float _quadratic)
    : Light(_diffuse, _ambient, _specular), position(_position), constant(_constant), linear(_linear), quadratic(_quadratic)
{
    const std::vector<PointLight*>& pointLights = SceneManager::GetCurrentScene()->GetPointLights();
    size_t maxPoints = SceneManager::GetCurrentScene()->GetMaxPointLights();

    id = pointLights.size();
    if (id >= maxPoints)
    {
        Log::Print("The PointLight %d will not be used. PointLights limit : %d", id, maxPoints);
    }
    else
    {
        SceneManager::GetCurrentScene()->AddLight(this);
        //SceneManager::GetCurrentScene()->GetPointLights().push_back(this);
       // TODO maybe for all lights
    }
}

PointLight::~PointLight()
{
    SceneManager::GetCurrentScene()->DeleteLight(this);
}

void PointLight::SetPointLight(Shader* _shader)
{
    _shader->GetFragmentShader()->SetVec3("pointLights[" + std::to_string(id) + "].position", position);
    _shader->GetFragmentShader()->SetVec4("pointLights[" + std::to_string(id) + "].ambient", ambientColor);
    _shader->GetFragmentShader()->SetVec4("pointLights[" + std::to_string(id) + "].diffuse", diffuseColor);
    _shader->GetFragmentShader()->SetVec4("pointLights[" + std::to_string(id) + "].specular", specularColor);
    _shader->GetFragmentShader()->SetFloat("pointLights[" + std::to_string(id) + "].constant", constant);
    _shader->GetFragmentShader()->SetFloat("pointLights[" + std::to_string(id) + "].linear", linear);
    _shader->GetFragmentShader()->SetFloat("pointLights[" + std::to_string(id) + "].quadratic", quadratic);
}

void PointLight::Update()
{
    if (owner)
        position = owner->transform->globalPosition;
}
