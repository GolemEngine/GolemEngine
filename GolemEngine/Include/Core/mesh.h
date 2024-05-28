#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "dll.h"
#include "Resource/Rendering/Shader/shader.h"
#include "Resource/Rendering/texture.h"
#include "Resource/Rendering/model.h"
#include "Core/gameobject.h"

using json = nlohmann::json;

class Transform;
class Camera;

class GOLEM_ENGINE_API Mesh
{
private:
	Texture* m_texture;
	Model* m_model;
	Shader* m_shader;

private:
	void SetupMesh();

public:
	Mesh(Model* _model, Texture* _texture, Shader* _shader);
	~Mesh();

	Texture* GetTexture();
	Model* GetModel();
	Shader* GetShader();


	// Define serialization and deserialization functions manually because the
	// macro is not used due to the pointer member variable.
	void ToJson(json& j) const
	{
		j = json
		{
			{"texturePath", m_texture->path},
			{"modelPath", m_model->path},
			{"shaderPath", m_shader->path}
		};
	}
};