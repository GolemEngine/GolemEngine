#pragma once

#include <vector>

#include <vector3.h>

#include "dll.h"
#include "Components/Light/Light.h"
#include "Refl/refl.hpp"
#include "Reflection/attributes.h"

class Shader;

class PointLight : public Light
{
public:
    size_t id;
    Vector3 position;
    float constant;
    float linear;
    float quadratic;

public:
    GOLEM_ENGINE_API PointLight();
    GOLEM_ENGINE_API PointLight(const Vector4& _diffuse, const Vector4& _ambient, const Vector4& _specular, const Vector3& _position,
        const float _constant, const float _linear, const float _quadratic);
    GOLEM_ENGINE_API~PointLight();
    GOLEM_ENGINE_API void SetPointLight(Shader* _shader);
   
    GOLEM_ENGINE_API void Update() override;

    
    // Define serialization and deserialization functions manually because the
    // macro is not used due to the pointer member variable.
    GOLEM_ENGINE_API void ToJson(json& j) const override
    {
        j = json
        {
            {"name", "pointLight"},
            {"data",
                {
                    {"id", id},
                    {"diffuseColor", diffuseColor},
                    {"ambientColor", ambientColor},
                    {"specularColor", specularColor},
                    {"position", position},
                    {"constant", constant},
                    {"linear", linear},
                    {"quadratic", quadratic}
                }
            }
        };
    }
};

REFL_AUTO(
    type(PointLight, bases<Light>),
    field(constant, Range(0, 1)),
    field(linear, Range(0, 1)),
    field(quadratic, Range(0, 1))
)