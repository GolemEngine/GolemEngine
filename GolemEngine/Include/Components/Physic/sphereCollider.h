#pragma once

#include "dll.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "Components/Physic/collider.h"
#include "Core/gameobject.h"
#include "Refl/refl.hpp"

using namespace JPH;

class SphereCollider : public Collider
{
public:
	float radius;
	
public:
	SphereCollider();
	SphereCollider(float _radius);
	~SphereCollider();

	void Begin() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void Draw(Camera* _camera);

	void ToJson(json& j) const {}

	friend refl_impl::metadata::type_info__<SphereCollider>; // needed to reflect private members
};

REFL_AUTO(
	type(SphereCollider, bases<Collider>),
	field(radius, Range(0.1f, 100))
)