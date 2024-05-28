#include "Core/mesh.h"

#include <MathsLib/utils.h>

#include "Wrappers/graphicWrapper.h"
#include "Components/transform.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Resource/Rendering/texture.h"
#include "Resource/Rendering/model.h"
#include "Core/camera.h"
#include "Resource/sceneManager.h"

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &m_model->VAO);
    glGenBuffers(1, &m_model->VBO);
    // Bindings
    glBindVertexArray(m_model->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_model->VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_model->vertices.size(), m_model->vertices.data(), GL_STATIC_DRAW);
    
    // Attribues
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);  
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(Model* _model, Texture* _texture, Shader* _shader)
    : m_model(_model), m_texture(_texture), m_shader(_shader)
{
    SetupMesh();
}

Mesh::~Mesh() 
{
    glDeleteVertexArrays(1, &m_model->VAO);
    glDeleteBuffers(1, &m_model->VBO);
}

Texture* Mesh::GetTexture()
{
    return m_texture;
}

Model* Mesh::GetModel()
{
    return m_model;
}

Shader* Mesh::GetShader()
{
    return m_shader;
}