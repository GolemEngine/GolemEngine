#pragma once

#include <vector>

#include "vector3.h"
#include "quaternion.h"
#include "matrix4.h"
#include "Components/component.h"
#include "Resource/guid.h"
#include "Core/camera.h"

class Transform : public Component
{
private:
	Transform* m_parent;
	std::vector<Transform*> m_children;
	Matrix4 m_localModel;
	Matrix4 m_globalModel;

public:
	Guid guid;

	Vector3 globalPosition; // access only, modification is useless yet
	Vector3 localPosition;
	Vector3 rotation;
	Vector3 scaling;
	Vector3 oldScaling;

public:
	GOLEM_ENGINE_API Transform();
	GOLEM_ENGINE_API Transform(Transform* _parent);
	GOLEM_ENGINE_API Transform(Vector3 _position, Vector3 _rotation, Vector3 _scaling, Transform* _parent);
	GOLEM_ENGINE_API ~Transform() override;

	GOLEM_ENGINE_API void Update() override;
	GOLEM_ENGINE_API void UpdateSelfAndChilds();
	GOLEM_ENGINE_API void EditTransformGizmo();
	GOLEM_ENGINE_API void AddChild(Transform* const _t);
	GOLEM_ENGINE_API void AddChildren(std::vector<Transform*> const _ts);
	GOLEM_ENGINE_API void RemoveChild(Transform* const _t);
	GOLEM_ENGINE_API void SetParent(Transform* const _t);
	GOLEM_ENGINE_API bool IsChildOf(Transform* const _parent);
	GOLEM_ENGINE_API Vector3 GetForward() const;
	_NODISCARD GOLEM_ENGINE_API bool IsAParentOf(Transform* _t);

	GOLEM_ENGINE_API Transform* GetParent();

	GOLEM_ENGINE_API Matrix4 GetGlobalModel();
	GOLEM_ENGINE_API Matrix4 GetLocalModel();

	GOLEM_ENGINE_API const std::vector<Transform*>& GetChildren() const;


	// Define serialization and deserialization functions manually because the
	// macro is not used due to the pointer member variable.
	void ToJson(json& _j) const override
	{
		_j = json
		{
			{"name", "transform"},
			{"data",
				{
					{"guid", guid.ToString()},
					{"globalPosition", globalPosition},
					{"localPosition", localPosition},
					{"rotation", rotation},
					{"scaling", scaling}
				}
			}
		};
	};
};

REFL_AUTO(
	type(Transform, bases<Component>),
	field(localPosition),
	field(rotation),
	field(scaling)
)