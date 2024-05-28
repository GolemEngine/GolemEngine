#include "Components/Physic/capsuleCollider.h"

#include <Jolt/Physics/Body/BodyInterface.h>

#include <iostream>

#include "Physic/physicSystem.h"
#include "Physic/layers.h"
#include "Wrappers/graphicWrapper.h"
#include "Resource/resourceManager.h"
#include "MathsLib/utils.h"
#include "Wrappers/windowWrapper.h"

CapsuleCollider::CapsuleCollider()
	: height(1.0f), radius(1.0f)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	SetModelPath("capsuleCollider.obj");
	SetModel(resourceManager->Get<Model>(GetModelPath()));
}

CapsuleCollider::CapsuleCollider(float _height, float _radius)
	: height(_height), radius(_radius)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	SetModelPath("capsuleCollider.obj");
	SetModel(resourceManager->Get<Model>(GetModelPath()));
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::Begin()
{
	id = PhysicSystem::CreateCapsuleCollider(owner->transform->localPosition, Quaternion::EulerToQuaternion(owner->transform->rotation), height, radius);
}

void CapsuleCollider::PreUpdate()
{
	Collider::PreUpdate();
}

void CapsuleCollider::Update()
{
	Collider::Update();
	PhysicSystem::SetCapsuleShape(id, height, radius);
}

void CapsuleCollider::PostUpdate()
{
	Collider::PostUpdate();
}

void CapsuleCollider::Draw(Camera* _camera)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	Shader* shader = resourceManager->Get<Shader>(resourceManager->GetColliderShader());
	Model* model = GetModel();

	shader->Use();

	Matrix4 view = _camera->GetViewMatrix();
	Matrix4 projection = Matrix4::Projection(DegToRad(_camera->GetZoom()), WindowWrapper::GetScreenSize().x / WindowWrapper::GetScreenSize().y, _camera->GetNear(), _camera->GetFar());
	Matrix4 modelMat = Matrix4::TRS(owner->transform->localPosition, Quaternion::EulerToQuaternion(owner->transform->rotation), Vector3(radius, height, radius));
	shader->GetVertexShader()->SetMat4("model", modelMat);
	shader->GetVertexShader()->SetMat4("view", view);
	shader->GetVertexShader()->SetMat4("projection", projection);

	glBindVertexArray(model->VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model->vertices.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}