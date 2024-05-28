#include "Resource/Rendering/Shader/computeShader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Debug/log.h"
#include "Utils/tools.h"
#include "Wrappers/graphicWrapper.h"


ComputeShader::ComputeShader() {}

void ComputeShader::Init()
{
    std::string computeCode;
    std::ifstream cShaderFile;
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        cShaderFile.open(m_computePath);
        std::stringstream cShaderStream;
        cShaderStream << cShaderFile.rdbuf();
        cShaderFile.close();
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        Log::Print("ERROR::SHADER::COMPUTE::FILE_NOT_SUCCESFULLY_READ\n");
    }

    const char* cShaderCode = computeCode.c_str();

    unsigned int compute;

    compute = GraphicWrapper::CreateShader(SHADER_TYPE_COMPUTE);
    GraphicWrapper::SetShaderSourceCode(compute, 1, &cShaderCode, NULL);
    GraphicWrapper::CompileShader(compute);
    Tools::CheckCompileErrors(compute, "COMPUTE");

    id = GraphicWrapper::CreateShaderProgram();    // Use a different program than the default shader
    GraphicWrapper::AttachShaderToProgram(id, compute);
    GraphicWrapper::LinkProgram(id);
    Tools::CheckCompileErrors(id, "PROGRAM");

    GraphicWrapper::DeleteShaderObject(compute);
}

void ComputeShader::Use()
{
    GraphicWrapper::UseShader(id);
}

void ComputeShader::SetComputePath(std::string _computePath)
{
    m_computePath = _computePath;
}

void ComputeShader::SetBool(const std::string& _name, bool _value) const
{
    GraphicWrapper::SetShaderBool(id, _name, _value);
}

void ComputeShader::SetInt(const std::string& _name, int _value) const
{
    GraphicWrapper::SetShaderInt(id, _name, _value);
}

void ComputeShader::SetFloat(const std::string& _name, float _value) const
{
    GraphicWrapper::SetShaderFloat(id, _name, _value);
}

void ComputeShader::SetVec2(const std::string& _name, const Vector2& _value) const
{
    GraphicWrapper::SetShaderVec2(id, _name, _value);
}

void ComputeShader::SetVec2(const std::string& _name, float _x, float _y) const
{
    GraphicWrapper::SetShaderVec2(id, _name, _x, _y);
}

void ComputeShader::SetVec3(const std::string& _name, const Vector3& _value) const
{
    GraphicWrapper::SetShaderVec3(id, _name, _value);
}

void ComputeShader::SetVec3(const std::string& _name, float _x, float _y, float _z) const
{
    GraphicWrapper::SetShaderVec3(id, _name, _x, _y, _z);
}

void ComputeShader::SetVec4(const std::string& _name, const Vector4& _value) const
{
    GraphicWrapper::SetShaderVec4(id, _name, _value);
}

void ComputeShader::SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const
{
    GraphicWrapper::SetShaderVec4(id, _name, _x, _y, _z, _w);
}

void ComputeShader::SetMat4(const std::string& _name, const Matrix4& _value) const
{
    GraphicWrapper::SetShaderMat4(id, _name, _value);
}

