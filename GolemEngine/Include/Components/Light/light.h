#pragma once

#include "dll.h"

#include <vector4.h>

#include "Resource/resource.h"
#include "Components/component.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Refl/refl.hpp"


class Light : public Component
{
public:
	Vector4 diffuseColor;
	Vector4 ambientColor;
	Vector4 specularColor;

public:
	GOLEM_ENGINE_API Light() = default;
	GOLEM_ENGINE_API Light(const Vector4& _diffuse, const Vector4& _ambient, const Vector4& _specular);
	GOLEM_ENGINE_API virtual ~Light();
	GOLEM_ENGINE_API virtual void Update() override {};
};

REFL_AUTO(
    type(Light, bases<Component>),
    field(diffuseColor),
	field(ambientColor),
	field(specularColor)
)