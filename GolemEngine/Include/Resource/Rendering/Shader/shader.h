#pragma once

#include <string>

#include "dll.h"
#include "fragmentShader.h"
#include "Resource/resource.h"
#include "vertexShader.h"
#include "fragmentShader.h"


class Shader : public Resource
{
private:
    VertexShader* m_vertexShader = nullptr;
    FragmentShader* m_fragmentShader = nullptr;
    
public:
    unsigned int id = 0;
    
public:
    GOLEM_ENGINE_API Shader();

    GOLEM_ENGINE_API void SetAllShaders(VertexShader* _vertexShader, FragmentShader* _fragmentShader);

    GOLEM_ENGINE_API void Use();

    GOLEM_ENGINE_API VertexShader* GetVertexShader();
    GOLEM_ENGINE_API FragmentShader* GetFragmentShader();
};