#include "Components/GameClasses/enemy.h"

Enemy::Enemy(Vector3 _initPosition, std::string _name, std::string _textureName, std::string _shaderName)
	: speed(0.1f)
{
	this->name = _name;

	ResourceManager* resourceManager = ResourceManager::GetInstance();

	transform->localPosition = _initPosition;
	transform->rotation = 0;

	texture = resourceManager->Get<Texture>("aoi_todo.jpg");
	shader = resourceManager->Get<Shader>(ResourceManager::GetDefaultShader());

	model = resourceManager->Get<Model>("sphere.obj");
	mesh = new Mesh(model, texture, shader);

	this->AddComponent(new MeshRenderer(mesh));

	m_sphereCollider = new SphereCollider();
	AddComponent(m_sphereCollider);
	m_sphereCollider->Begin();
	m_sphereCollider->gravityFactor = 0;
	m_sphereCollider->motionType = MotionType::Dynamic;
	m_sphereCollider->isActivated = true;
}

void Enemy::Update()
{
}
