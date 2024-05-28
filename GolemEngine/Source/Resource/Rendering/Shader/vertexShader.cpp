#include "Resource/Rendering/Shader/vertexShader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Resource/Rendering/Shader/shader.h"
#include "Debug/log.h"
#include "Utils/tools.h"
#include "Wrappers/graphicWrapper.h"


VertexShader::VertexShader(Shader* _shader, const char* _vertexPath)
    : m_shader(_shader), m_vertexPath(_vertexPath)
{
    Init();
}

void VertexShader::Init()
{
    std::string vertexCode;
    std::ifstream vShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_vertexPath;
    try
    {
        vShaderFile.open(m_vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        Log::Print("ERROR::SHADER::VERTEX::FILE_NOT_SUCCESFULLY_READ\n");
    }

    const char* vShaderCode = vertexCode.c_str();

    unsigned int vertex;

    vertex = GraphicWrapper::CreateShader(SHADER_TYPE_VERTEX);
    GraphicWrapper::SetShaderSourceCode(vertex, 1, &vShaderCode, NULL);
    GraphicWrapper::CompileShader(vertex);
    Tools::CheckCompileErrors(vertex, "VERTEX");

    GraphicWrapper::AttachShaderToProgram(m_shader->id, vertex);
    GraphicWrapper::LinkProgram(m_shader->id);
    Tools::CheckCompileErrors(m_shader->id, "PROGRAM");

    GraphicWrapper::DeleteShaderObject(vertex);
}

void VertexShader::SetBool(const std::string& _name, bool _value) const
{
    GraphicWrapper::SetShaderBool(m_shader->id, _name, _value);
}

void VertexShader::SetInt(const std::string& _name, int _value) const
{
    GraphicWrapper::SetShaderInt(m_shader->id, _name, _value);
}

void VertexShader::SetFloat(const std::string& _name, float _value) const
{
    GraphicWrapper::SetShaderFloat(m_shader->id, _name, _value);
}

void VertexShader::SetVec2(const std::string& _name, const Vector2& _value) const
{
    GraphicWrapper::SetShaderVec2(m_shader->id, _name, _value);
}

void VertexShader::SetVec2(const std::string& _name, float _x, float _y) const
{
    GraphicWrapper::SetShaderVec2(m_shader->id, _name, _x, _y);
}

void VertexShader::SetVec3(const std::string& _name, const Vector3& _value) const
{
    GraphicWrapper::SetShaderVec3(m_shader->id, _name, _value);
}

void VertexShader::SetVec3(const std::string& _name, float _x, float _y, float _z) const
{
    GraphicWrapper::SetShaderVec3(m_shader->id, _name, _x, _y, _z);
}

void VertexShader::SetVec4(const std::string& _name, const Vector4& _value) const
{
    GraphicWrapper::SetShaderVec4(m_shader->id, _name, _value);
}

void VertexShader::SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const
{
    GraphicWrapper::SetShaderVec4(m_shader->id, _name, _x, _y, _z, _w);
}

void VertexShader::SetMat4(const std::string& _name, const Matrix4& _value) const
{
    GraphicWrapper::SetShaderMat4(m_shader->id, _name, _value);
}

