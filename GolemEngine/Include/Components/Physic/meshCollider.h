#pragma once

#include "dll.h"

#include <Jolt/Jolt.h>
#include "Components/Physic/collider.h"
#include "Core/gameobject.h"
#include "Core/camera.h"
#include "Refl/refl.hpp"

using namespace JPH;

class MeshCollider : public Collider
{
public:
	MeshCollider();
	~MeshCollider();

	void Begin() override;
	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void Draw(Camera* _camera);

	void ToJson(json& j) const {}

	friend refl_impl::metadata::type_info__<MeshCollider>; // needed to reflect private members
};

REFL_AUTO(
	type(MeshCollider, bases<Collider>)
)