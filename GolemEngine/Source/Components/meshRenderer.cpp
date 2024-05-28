#include "Components/meshRenderer.h"

#include "Wrappers/graphicWrapper.h"
#include "MathsLib/utils.h"
#include "Resource/Rendering/texture.h"
#include "Resource/Rendering/model.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Utils/viewportTools.h"
#include "Wrappers/windowWrapper.h"


MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* _mesh)
    : m_mesh(_mesh)
{
}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::Draw(Camera* _camera)
{   
    if (m_mesh)
    {
        Texture* texture = m_mesh->GetTexture();
        Model* model = m_mesh->GetModel();
        Shader* shader = m_mesh->GetShader();

        glActiveTexture(GL_TEXTURE0);
        texture->Use();

        shader->Use();
        shader->GetVertexShader()->SetMat4("model", owner->transform->GetGlobalModel());
        shader->GetVertexShader()->SetMat4("view", _camera->GetViewMatrix());
        shader->GetVertexShader()->SetMat4("projection", Matrix4::Projection(DegToRad(_camera->GetZoom()), WindowWrapper::GetScreenSize().x / WindowWrapper::GetScreenSize().y, _camera->GetNear(), _camera->GetFar()));

        glBindVertexArray(model->VAO);

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
        
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model->vertices.size());
        glBindVertexArray(0);
        texture->UnUse();
        // Reset to fill
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

Mesh* MeshRenderer::GetMesh()
{
    return m_mesh;
}
