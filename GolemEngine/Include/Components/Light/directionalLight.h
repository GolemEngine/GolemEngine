#pragma once

#include <vector>
#include <vector3.h>

#include "dll.h"
#include "Components/Light/Light.h"
#include "Refl/refl.hpp"

class Shader;

class DirectionalLight : public Light
{
public:
	size_t id;
	Vector3 direction;

public:
	GOLEM_ENGINE_API DirectionalLight();
	GOLEM_ENGINE_API DirectionalLight(const Vector4& _diffuse, const Vector4& _ambient, const Vector4& _specular, const Vector3& _direction);
	GOLEM_ENGINE_API ~DirectionalLight();
	GOLEM_ENGINE_API void SetDirectionalLight(Shader* _shader);
	
	GOLEM_ENGINE_API void Update() override;


	// Define serialization and deserialization functions manually because the
	// macro is not used due to the pointer member variable.
	GOLEM_ENGINE_API void ToJson(json& j) const override
	{
		j = json
		{
			{"name", "directionalLight"},
			{"data",
				{
					{"id", id},
					{"diffuseColor", diffuseColor},
					{"ambientColor", ambientColor},
					{"specularColor", specularColor},
					{"direction", direction}
				}
			}
		};
	}
};

REFL_AUTO(
	type(DirectionalLight, bases<Light>),
	field(direction)
)