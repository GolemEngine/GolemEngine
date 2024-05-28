#include "WorldBuilder/terrain.h"

#include <utils.h>

#include "golemEngine.h"
#include "imgui.h"
#include "Components/Light/directionalLight.h"
#include "Components/Light/pointLight.h"
#include "Components/Light/spotLight.h"
#include "Wrappers/graphicWrapper.h"
#include "Resource/resourceManager.h"
#include "Resource/sceneManager.h"
#include "Wrappers/windowWrapper.h"
#include "Utils/viewportTools.h"
#include "Resource/Rendering/Shader/shader.h"
#include "WorldBuilder/worldBuilder.h"


Terrain::Terrain(std::string _name, Transform* _transform)
    : GameObject(_name, _transform)
{
    // Set shader
    ResourceManager* resourceManager = ResourceManager::GetInstance();
    m_shader = resourceManager->Get<Shader>(ResourceManager::GetTerrainShader());
    m_shaderTerrainUv = resourceManager->Get<Shader>(ResourceManager::GetTerrainUvShader());
    m_computeShader = resourceManager->Get<ComputeShader>(ResourceManager::GetTerrainComputeShader());

    m_texture0 = resourceManager->Get<Texture>(ResourceManager::GetGridTerrainTexture());
    m_texture1 = resourceManager->Get<Texture>(ResourceManager::GetDirtTexture());
    m_texture2 = resourceManager->Get<Texture>(ResourceManager::GetGrassTexture());
    m_texture3 = resourceManager->Get<Texture>(ResourceManager::GetSnowTexture());

    isTerrain = true;
}

Terrain::~Terrain()
{
    DeleteAllComponents();
}

void Terrain::SetupMesh()
{
    // Setup the compute shader
    glGenBuffers(1, &m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glNamedBufferStorage(m_ssbo, sizeof(VertexGpu) * m_vertices.size(), (const void*)m_vertices.data(), GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
}

void Terrain::UseComputeShader(Vector2 _mousePos)
{
    m_computeShader->Use();

    m_computeShader->SetVec2("brushPosition", _mousePos);
    m_computeShader->SetBool("isBrushActive", WorldBuilder::brush->isActive);
    m_computeShader->SetFloat("brushRadius", WorldBuilder::brush->radius);
    m_computeShader->SetFloat("brushForce", WorldBuilder::brush->force);
    m_computeShader->SetVec2("terrainScale", {transform->scaling.x, transform->scaling.z});
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glDispatchCompute(m_vertices.size(), 1, 1);
    
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    m_computeShader->SetBool("isBrushActive", false);
}

void Terrain::Draw(Camera* _camera)
{
    m_shader->Use();
    m_shader->GetVertexShader()->SetMat4("model", transform->GetGlobalModel());
    m_shader->GetVertexShader()->SetMat4("view", _camera->GetViewMatrix());
    m_shader->GetVertexShader()->SetMat4("projection", Matrix4::Projection(DegToRad(_camera->GetZoom()), WindowWrapper::GetScreenSize().x / WindowWrapper::GetScreenSize().y, _camera->GetNear(), _camera->GetFar()));
    m_shader->GetVertexShader()->SetFloat("minHeight", yMin);
    m_shader->GetVertexShader()->SetFloat("maxHeight", yMax);
    
    if (m_texture0 == nullptr)
    {
        useGradient = true;
    }

    if (useGradient)
    {
        m_shader->GetFragmentShader()->SetBool("useTexture", false);
        m_shader->GetFragmentShader()->SetInt("ourTexture0", -1);
        m_shader->GetFragmentShader()->SetInt("ourTexture1", -1);
    }
    else
    {
        m_shader->GetFragmentShader()->SetBool("useTexture", true);
        
        glActiveTexture(GL_TEXTURE0);
        m_texture0->Use();
        m_shader->GetFragmentShader()->SetInt("ourTexture0", 0);
        glActiveTexture(GL_TEXTURE1);
        m_texture1->Use();
        m_shader->GetFragmentShader()->SetInt("ourTexture1", 1);
        m_shader->GetFragmentShader()->SetFloat("texture1Level", WorldBuilder::texture1Level);
        glActiveTexture(GL_TEXTURE2);
        m_texture2->Use();
        m_shader->GetFragmentShader()->SetInt("ourTexture2", 2);
        m_shader->GetFragmentShader()->SetFloat("texture2Level", WorldBuilder::texture2Level);
        glActiveTexture(GL_TEXTURE3);
        m_texture3->Use();
        m_shader->GetFragmentShader()->SetInt("ourTexture3", 3);
    }

    if (!SceneManager::GetCurrentScene()->GetDirectionalLights().empty() ||
        !SceneManager::GetCurrentScene()->GetPointLights().empty() ||
        !SceneManager::GetCurrentScene()->GetSpotLights().empty())
    {
        UpdateLights(m_shader);
    }
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    
    // Switch draw mode depending on view mode
    switch (ViewportTools::currentViewMode)
    {
    case DEFAULT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

    case WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }

    Matrix4 modelMatrix = transform->GetGlobalModel();
    if (modelMatrix != m_oldModelMatrix)
    {
        RetrieveComputeData(_camera);
    }
    
    glUseProgram(m_shader->id);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    // Reset to fill
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::UpdateLights(Shader* _shader)
{
    _shader->Use();

    _shader->GetFragmentShader()->SetInt("nbrDirLights", (int)SceneManager::GetCurrentScene()->GetDirectionalLights().size());
    _shader->GetFragmentShader()->SetInt("nbrPointLights", (int)SceneManager::GetCurrentScene()->GetPointLights().size());
    _shader->GetFragmentShader()->SetInt("nbrSpotLights", (int)SceneManager::GetCurrentScene()->GetSpotLights().size());

    for (unsigned int i = 0; i < SceneManager::GetCurrentScene()->GetDirectionalLights().size(); ++i)
    {
        SceneManager::GetCurrentScene()->GetDirectionalLights()[i]->SetDirectionalLight(_shader);
    }
    for (unsigned int i = 0; i < SceneManager::GetCurrentScene()->GetPointLights().size(); ++i)
    {
        SceneManager::GetCurrentScene()->GetPointLights()[i]->SetPointLight(_shader);
    }
    for (unsigned int i = 0; i < SceneManager::GetCurrentScene()->GetSpotLights().size(); ++i)
    {
        SceneManager::GetCurrentScene()->GetSpotLights()[i]->SetSpotLight(_shader);
    }
}

void Terrain::CalculateNormals()
{
    for (int i = 0; i < xResolution; ++i)
    {
        for (int j = 0; j < zResolution; ++j)
        {
            Vector3 normal(0.0f, 0.0f, 0.0f);
            Vector3 currentPosition = m_vertices[j + i * zResolution].position;

            if (i < xResolution - 1 && j < zResolution - 1) // Bottom-right triangle
            {
                Vector3 bottomPosition = m_vertices[j + 1 + i * zResolution].position;
                Vector3 rightPosition = m_vertices[j + (i + 1) * zResolution].position;
                Vector3 bottom = bottomPosition - currentPosition;
                Vector3 right = rightPosition - currentPosition;
                normal += Vector3::Cross(bottom, right);
            }
            if (i < xResolution - 1 && j > 0) // Top-right triangle
            {
                Vector3 topPosition = m_vertices[j - 1 + i * zResolution].position;
                Vector3 rightPosition = m_vertices[j + (i + 1) * zResolution].position;
                Vector3 top = topPosition - currentPosition;
                Vector3 right = rightPosition - currentPosition;
                normal += Vector3::Cross(top, right);
            }
            if (i > 0 && j < zResolution - 1) // Bottom-left triangle
            {
                Vector3 bottomPosition = m_vertices[j + 1 + i * zResolution].position;
                Vector3 leftPosition = m_vertices[j + (i - 1) * zResolution].position;
                Vector3 bottom = bottomPosition - currentPosition;
                Vector3 left = leftPosition - currentPosition;
                normal += Vector3::Cross(bottom, left);
            }
            if (i > 0 && j > 0) // Top-left triangle
            {
                Vector3 topPosition = m_vertices[j - 1 + i * zResolution].position;
                Vector3 leftPosition = m_vertices[j + (i - 1) * zResolution].position;
                Vector3 top = topPosition - currentPosition;
                Vector3 left = leftPosition - currentPosition;
                normal += Vector3::Cross(left, top);
            }

            m_vertices[j + i * zResolution].normal = normal.Normalize();
        }
    }
}

void Terrain::RetrieveComputeData(Camera* _camera)
{
    // Bind the SSBO containing the vertex data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo); 
    // Create a buffer to store the output of the gpu
    GLint bufferSize;
    glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    // Store the output to the buffer
    std::vector<VertexGpu> verticesOut(bufferSize / sizeof(VertexGpu));
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, bufferSize, verticesOut.data());
    
    // Ensure m_vertices has the same size as the output data
    if (m_vertices.size() != verticesOut.size())
    {
        m_vertices.resize(verticesOut.size());
    }

    // Copy the data from the temporary buffer to m_vertices
    std::copy(verticesOut.begin(), verticesOut.end(), m_vertices.begin());

    UpdateVertices(_camera);
}

void Terrain::UpdateVertices(Camera* _camera)
{
    if (!m_vertices.empty())
    {
        // Get the model matrix to use them for calculation after
        Matrix4 modelMatrix = transform->GetGlobalModel();
    
        float minY = std::numeric_limits<float>::max();    // So that any value found in the loop HAS TO BE SMALLER than this
        float maxY = std::numeric_limits<float>::lowest();    // So that any value found in the loop HAS TO BE BIGGER than this
    
        if (m_oldModelMatrix != modelMatrix)
        {
            for (int i = 0; i < m_vertices.size(); ++i)
            {
                // Get the original vertex position
                Vector4 originalPosition = Vector4(m_vertices[i].position.x, m_vertices[i].position.y, m_vertices[i].position.z, 1.0f);
                // Apply the model matrix
                Vector4 transformedPosition = modelMatrix * originalPosition;
                // Set final position
                Vector3 finalPosition = Vector3(transformedPosition.x, transformedPosition.y, transformedPosition.z);
                // Update the vertex position in the CPU buffer
                m_vertices[i].position = finalPosition;
                // Set min and max point of the heightmap
                minY = std::min(minY, finalPosition.y);
                maxY = std::max(maxY, finalPosition.y);
            }
            yMin = minY;
            yMax = maxY;
        }
    
        m_oldModelMatrix = modelMatrix;
    
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    else
    {
        std::cout << "ERROR: vertex output from the compute shader is null" << std::endl;
    }
    
    // Don't forget to unbind the buffer after use
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::vector<Vertex> Terrain::GetVertices()
{
    std::vector<Vertex> vertices;
    for (VertexGpu v : m_vertices)
    {
        Vertex vert = {v.position, v.normal, v.textureCoords};
        vertices.push_back(vert);
    }
    return vertices;
}

std::vector<VertexGpu> Terrain::GetVerticesGpu()
{
    return m_vertices;
}

Shader* Terrain::GetShader()
{
    return  m_shader;
}

ComputeShader* Terrain::GetComputeShader()
{
    return m_computeShader;
}
