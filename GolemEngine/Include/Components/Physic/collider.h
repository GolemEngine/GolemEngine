#pragma once

#include "Components/component.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "Physic/physicSystem.h"
#include "Components/component.h"
#include "Resource/Rendering/model.h"
#include "Core/camera.h"
#include "Refl/refl.hpp"

using namespace JPH;

class Collider : public Component
{
private:
	Model* m_model = nullptr;
	const char* m_modelPath;

public:
	BodyID id;
	bool isActivated;
	MotionType motionType;
	float gravityFactor;
	ColliderState colliderState;
	std::function<void (Collider* _collider, Collider* _other)> onCollisionEnter;
	std::function<void (Collider* _collider, Collider* _other)> onCollisionStay;
	std::function<void(Collider* _collider, Collider* _other)> onCollisionExit;

public:
	Collider();
	virtual ~Collider();

	virtual void Begin() override;

	virtual void PreUpdate();
	virtual void Update() override;
	virtual void PostUpdate();

	virtual void Draw(Camera* _camera) {};

	virtual void ToJson(json& j) const {}

	Model* GetModel();
	void SetModel(Model* _model); 
	const char* GetModelPath();
	void SetModelPath(const char* _path);

	void OnCollisionEnter(Collider* _other);
	void OnCollisionStay(Collider* _other);
	void OnCollisionExit(Collider* _other);

	friend refl_impl::metadata::type_info__<Collider>; // needed to reflect private members
};

REFL_AUTO(
	type(Collider, bases<Component>),
	field(isActivated),
	field(motionType),
	field(gravityFactor)
)