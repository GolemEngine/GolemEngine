#include "Components/Physic/collider.h"
#include "Physic/physicSystem.h"
#include "Core/gameobject.h"
#include "Wrappers/graphicWrapper.h"

Collider::Collider()
	: isActivated(false), motionType(MotionType::Static), m_model(nullptr), m_modelPath(nullptr), colliderState(ColliderState::DEFAULT), gravityFactor(1.0f)
{
	PhysicSystem::AddCollider(this);
}

Collider::~Collider()
{
	PhysicSystem::DeleteBody(id);
	PhysicSystem::DeleteCollider(this);
}

void Collider::Begin()
{
}

void Collider::PreUpdate()
{
	switch (motionType)
	{
	case MotionType::Static:
		PhysicSystem::MakeBodyStatic(id);
		break;
	case MotionType::Kinematic:
		break;
	case MotionType::Dynamic:
		PhysicSystem::MakeBodyDynamic(id);
		break;
	default:
		break;
	}
	if (isActivated)
	{
		PhysicSystem::ActivateBody(id);
	}
	else
	{
		PhysicSystem::DeactivateBody(id, owner->transform->localPosition);
	}

	PhysicSystem::SetPosition(id, owner->transform->localPosition);
	PhysicSystem::SetRotation(id, owner->transform->rotation);
}

void Collider::Update()
{
	PhysicSystem::SetGravityFactor(id, gravityFactor);
}

void Collider::PostUpdate()
{
	PhysicSystem::UpdatePosition(id, owner->transform->localPosition);
}

Model* Collider::GetModel()
{
	return m_model;
}

void Collider::SetModel(Model* _model)
{
	m_model = _model;
	glGenVertexArrays(1, &m_model->VAO);

	glGenBuffers(1, &m_model->VBO);

	glBindVertexArray(m_model->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_model->VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_model->vertices.size(), m_model->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const char* Collider::GetModelPath()
{
	return m_modelPath;
}

void Collider::SetModelPath(const char* _path)
{
	m_modelPath = _path;
}

void Collider::OnCollisionEnter(Collider* _other)
{
	if (onCollisionEnter)
		onCollisionEnter(this, _other);
}

void Collider::OnCollisionStay(Collider* _other)
{
	if (onCollisionStay)
		onCollisionStay(this, _other);
}

void Collider::OnCollisionExit(Collider* _other)
{
	if (onCollisionExit)
		onCollisionExit(this, _other);
}

