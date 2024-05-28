#pragma once 
#include "dll.h"]
#include "Core/scene.h"
#include "Core/gameobject.h"
#include "Resource/sceneManager.h"
#include "Resource/resourceManager.h"
#include "Components/meshRenderer.h"
#include "Components/component.h"
#include "Components/Physic/sphereCollider.h"
#include "Components/Physic/boxCollider.h"
#include "Core/mesh.h"
#include "vector3.h"
#include <string>

class GOLEM_ENGINE_API Enemy : public GameObject
{
private:
	SphereCollider* m_sphereCollider;
public:
	Texture* texture;
	Shader* shader;
	Model* model;
	Mesh* mesh;

	Vector3 forward;

	Enemy(Vector3 _initPosition, std::string _name = "Enemy", std::string _textureName = "", std::string _shaderName = "");

	void Update() override;

	float speed;
};