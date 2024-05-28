#include "Components/GameClasses/playerMovement.h"
#include "Inputs/inputManager.h"

#include <iostream>

#include "Core/gameobject.h"
#include "golemEngine.h"

PlayerMovement::PlayerMovement()
{
}

PlayerMovement::~PlayerMovement()
{
}


void PlayerMovement::Begin()
{
	m_capsuleCollider = owner->GetComponent<CapsuleCollider>();
	m_capsuleCollider->motionType = MotionType::Dynamic;
	m_capsuleCollider->isActivated = true;
	m_capsuleCollider->onCollisionEnter = [this](Collider* _collider, Collider* _other) -> void { OnCollisionEnter(m_capsuleCollider, _other); };
}

void PlayerMovement::Update()
{
	MoveArround();
}

void PlayerMovement::MoveArround()
{
    Vector3 frontVector = GolemEngine::GetPlayerCamera()->GetFront();
    Vector3 rightVector = GolemEngine::GetPlayerCamera()->GetRight();

    if (InputManager::IsKeyPressed(KEY_W))
        movement += frontVector;
    if (InputManager::IsKeyPressed(KEY_S))
        movement += Vector3(0) - frontVector;
    if (InputManager::IsKeyPressed(KEY_A))
        movement += Vector3(0) - rightVector;
    if (InputManager::IsKeyPressed(KEY_D))
        movement += rightVector;

    movement = movement.Normalize();

    Vector3 current_velocity = PhysicSystem::GetVelocity(m_capsuleCollider->id);
    Vector3 desired_velocity = movement * moveSpeed;
    desired_velocity.y = current_velocity.y;
    Vector3 new_velocity = current_velocity * 0.75f + desired_velocity * 0.25f;
    PhysicSystem::SetVelocity(m_capsuleCollider->id, new_velocity);

    movement = Vector3(0);
}


void PlayerMovement::OnCollisionEnter(Collider* _collider, Collider* _other)
{
	PhysicSystem::AddForce(_collider->id, Vector3(0, 1, 0), bounceForce);
}
