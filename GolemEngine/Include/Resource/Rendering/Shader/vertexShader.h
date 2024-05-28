#pragma once

#include <string>

#include "dll.h"

class Shader;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;


class GOLEM_ENGINE_API VertexShader
{
private:
    Shader* m_shader = nullptr;
    const char* m_vertexPath;
    
public:
    VertexShader(Shader* _shader, const char* _vertexPath);

    void Init();

    void SetBool(const std::string& _name, bool _value) const;
    void SetInt(const std::string& _name, int _value) const;
    void SetFloat(const std::string& _name, float _value) const;
    void SetVec2(const std::string& _name, const Vector2& _value) const;
    void SetVec2(const std::string& _name, float _x, float _y) const;
    void SetVec3(const std::string& _name, const Vector3& _value) const;
    void SetVec3(const std::string& _name, float _x, float _y, float _z) const;
    void SetVec4(const std::string& _name, const Vector4& _value) const;
    void SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const;
    void SetMat4(const std::string& _name, const Matrix4& _mat) const;
};