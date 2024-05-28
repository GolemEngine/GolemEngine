#include "Resource/Rendering/Shader/fragmentShader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Resource/Rendering/Shader/shader.h"
#include "Debug/log.h"
#include "Utils/tools.h"
#include "Wrappers/graphicWrapper.h"


FragmentShader::FragmentShader(Shader* _shader, const char* _fragmentPath)
    : m_shader(_shader), m_fragmentPath(_fragmentPath)
{
    Init();
}

void FragmentShader::Init()
{
    std::string fragmentCode;
    std::ifstream fShaderFile;
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fShaderFile.open(m_fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    { 
        Log::Print("ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESFULLY_READ\n");
    }

    const char* fShaderCode = fragmentCode.c_str();

    unsigned int fragment;

    fragment = GraphicWrapper::CreateShader(SHADER_TYPE_FRAGMENT);
    GraphicWrapper::SetShaderSourceCode(fragment, 1, &fShaderCode, NULL);
    GraphicWrapper::CompileShader(fragment);
    Tools::CheckCompileErrors(fragment, "FRAGMENT");

    GraphicWrapper::AttachShaderToProgram(m_shader->id, fragment);
    GraphicWrapper::LinkProgram(m_shader->id);
    Tools::CheckCompileErrors(m_shader->id, "PROGRAM");

    GraphicWrapper::DeleteShaderObject(fragment);
}

void FragmentShader::SetBool(const std::string& _name, bool _value) const
{
    GraphicWrapper::SetShaderBool(m_shader->id, _name, _value);
}

void FragmentShader::SetInt(const std::string& _name, int _value) const
{
    GraphicWrapper::SetShaderInt(m_shader->id, _name, _value);
}

void FragmentShader::SetFloat(const std::string& _name, float _value) const
{
    GraphicWrapper::SetShaderFloat(m_shader->id, _name, _value);
}

void FragmentShader::SetVec2(const std::string& _name, const Vector2& _value) const
{
    GraphicWrapper::SetShaderVec2(m_shader->id, _name, _value);
}

void FragmentShader::SetVec2(const std::string& _name, float _x, float _y) const
{
    GraphicWrapper::SetShaderVec2(m_shader->id, _name, _x, _y);
}

void FragmentShader::SetVec3(const std::string& _name, const Vector3& _value) const
{
    GraphicWrapper::SetShaderVec3(m_shader->id, _name, _value);
}

void FragmentShader::SetVec3(const std::string& _name, float _x, float _y, float _z) const
{
    GraphicWrapper::SetShaderVec3(m_shader->id, _name, _x, _y, _z);
}

void FragmentShader::SetVec4(const std::string& _name, const Vector4& _value) const
{
    GraphicWrapper::SetShaderVec4(m_shader->id, _name, _value);
}

void FragmentShader::SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const
{
    GraphicWrapper::SetShaderVec4(m_shader->id, _name, _x, _y, _z, _w);
}

void FragmentShader::SetMat4(const std::string& _name, const Matrix4& _value) const
{
    GraphicWrapper::SetShaderMat4(m_shader->id, _name, _value);
}

