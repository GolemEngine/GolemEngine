#include "Components/Physic/meshCollider.h"

#include <Jolt/Physics/Body/BodyInterface.h>

#include <iostream>

#include "Physic/physicSystem.h"
#include "Physic/layers.h"
#include "Wrappers/graphicWrapper.h"
#include "Resource/resourceManager.h"
#include "Components/meshRenderer.h"
#include "MathsLib/utils.h"
#include "Wrappers/windowWrapper.h"
#include "WorldBuilder/terrain.h"

MeshCollider::MeshCollider()
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::Begin()
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	if (owner->GetComponent<MeshRenderer>() != nullptr)
	{
		Model* model = owner->GetComponent<MeshRenderer>()->GetMesh()->GetModel();
		Model* newModel = resourceManager->Create<Model>(owner->name + "Collider", model->path);
		newModel->Load(newModel->path.c_str());

		SetModel(newModel);
		id = PhysicSystem::CreateMeshCollider(GetModel()->vertices, owner->transform->localPosition, Quaternion::EulerToQuaternion(owner->transform->rotation));
	}
	else if (Terrain* terrain = dynamic_cast<Terrain*>(owner))
	{
		Model* newModel = resourceManager->Create<Model>(owner->name + "Collider", "");
		newModel->Load(terrain->GetVertices());

		SetModel(newModel);
		id = PhysicSystem::CreateMeshCollider(GetModel()->vertices, owner->transform->localPosition, Quaternion::EulerToQuaternion(owner->transform->rotation));
	}
	else
	{
		Log::Print("Can't set a mesh collider to an object which don't have a model");
	}
}

void MeshCollider::PreUpdate()
{
	Collider::PreUpdate();
}

void MeshCollider::Update()
{
	Collider::Update();
	if (owner->transform->oldScaling != owner->transform->scaling)
	{
		PhysicSystem::SetConvexHullShape(id, GetModel()->vertices);
	}
}

void MeshCollider::PostUpdate()
{
	Collider::PostUpdate();
}

void MeshCollider::Draw(Camera* _camera)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	Shader* shader = resourceManager->Get<Shader>(resourceManager->GetColliderShader());

	Model* model = GetModel();

	shader->Use();

	Matrix4 view = _camera->GetViewMatrix();
	Matrix4 projection = Matrix4::Projection(DegToRad(_camera->GetZoom()), WindowWrapper::GetScreenSize().x / WindowWrapper::GetScreenSize().y, _camera->GetNear(), _camera->GetFar());
	Matrix4 modelMat = Matrix4::TRS(owner->transform->localPosition, Quaternion::EulerToQuaternion(owner->transform->rotation), owner->transform->scaling);
	shader->GetVertexShader()->SetMat4("model", modelMat);
	shader->GetVertexShader()->SetMat4("view", view);
	shader->GetVertexShader()->SetMat4("projection", projection);

	glBindVertexArray(model->VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model->vertices.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}