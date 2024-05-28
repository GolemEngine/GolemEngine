#pragma once

#pragma once

#include "dll.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include "Components/Physic/collider.h"
#include "Core/gameobject.h"
#include "Core/camera.h"
#include "Refl/refl.hpp"

using namespace JPH;

class CapsuleCollider : public Collider
{
public:
	float height;
	float radius;
public:
	CapsuleCollider();
	CapsuleCollider(float _height, float _radius);
	~CapsuleCollider();

	void Begin() override;
	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void Draw(Camera* _camera);

	void ToJson(json& j) const {}

	friend refl_impl::metadata::type_info__<CapsuleCollider>; // needed to reflect private members
};

REFL_AUTO(
	type(CapsuleCollider, bases<Collider>),
	field(height, Range(0.1f, 100.f)),
	field(radius, Range(0.1f, 100.f))
)