#include "Resource/Rendering/Shader/shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Wrappers/graphicWrapper.h"
#include "Debug/log.h"
#include "Resource/Rendering/Shader/vertexShader.h"
#include "Resource/Rendering/Shader/fragmentShader.h"


Shader::Shader()
    : id(GraphicWrapper::CreateShaderProgram())
{}

void Shader::SetAllShaders(VertexShader* _vertexShader, FragmentShader* _fragmentShader)
{
    m_vertexShader = _vertexShader;
    m_fragmentShader = _fragmentShader;
}

void Shader::Use()
{
    GraphicWrapper::UseShader(id);
}

VertexShader* Shader::GetVertexShader()
{
    return m_vertexShader;
}

FragmentShader* Shader::GetFragmentShader()
{
    return m_fragmentShader;
}