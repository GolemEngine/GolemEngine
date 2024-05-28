#include "Components/Light/light.h"

Light::Light(const Vector4& _diffuse, const Vector4& _ambient, const Vector4& _specular)
	: diffuseColor(_diffuse), ambientColor(_ambient), specularColor(_specular)
{
}

Light::~Light() {}
