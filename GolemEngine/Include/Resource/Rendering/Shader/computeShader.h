#pragma once

// #include <string>

#include "dll.h"
#include "Resource/resource.h"

class Vector2;
class Vector3;
class Vector4;
class Matrix4;


class ComputeShader : public Resource
{
public:
    unsigned int id = 0;
    std::string m_computePath;

public:
    GOLEM_ENGINE_API ComputeShader();

    GOLEM_ENGINE_API void Init();
    GOLEM_ENGINE_API void Use();

    GOLEM_ENGINE_API void SetComputePath(std::string _computePath);

    GOLEM_ENGINE_API void SetBool(const std::string& _name, bool _value) const;
    GOLEM_ENGINE_API void SetInt(const std::string& _name, int _value) const;
    GOLEM_ENGINE_API void SetFloat(const std::string& _name, float _value) const;
    GOLEM_ENGINE_API void SetVec2(const std::string& _name, const Vector2& _value) const;
    GOLEM_ENGINE_API void SetVec2(const std::string& _name, float _x, float _y) const;
    GOLEM_ENGINE_API void SetVec3(const std::string& _name, const Vector3& _value) const;
    GOLEM_ENGINE_API void SetVec3(const std::string& _name, float _x, float _y, float _z) const;
    GOLEM_ENGINE_API void SetVec4(const std::string& _name, const Vector4& _value) const;
    GOLEM_ENGINE_API void SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const;
    GOLEM_ENGINE_API void SetMat4(const std::string& _name, const Matrix4& _mat) const;
};