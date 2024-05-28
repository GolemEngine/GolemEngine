#pragma once

#include "Components/script.h"
#include "Components/Physic/capsuleCollider.h"
#include "Refl/refl.hpp"

class PlayerMovement : public Script
{
private:
	CapsuleCollider* m_capsuleCollider;

public:
	Vector3 movement;
	float moveSpeed = 1.0f;
	float bounceForce = 5.0f;
public:
	PlayerMovement();
	~PlayerMovement();

	void Begin() override;
	void Update() override;

	void MoveArround();
	void OnCollisionEnter(Collider* _collider, Collider* _other);

	void ToJson(json& j) const {}
};

REFL_AUTO(
	type(PlayerMovement, bases<Script>),
	field(moveSpeed),
	field(bounceForce)
)