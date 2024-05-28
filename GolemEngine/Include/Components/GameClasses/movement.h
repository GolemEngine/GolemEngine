#pragma once
#include "dll.h"
#include "Refl/refl.hpp"
#include "Components/component.h"
#include "Core/gameobject.h"
#include "vector3.h"

class Movement : public Component
{
private:
	friend refl_impl::metadata::type_info__<Movement>;
public:
    GOLEM_ENGINE_API Movement();
    GOLEM_ENGINE_API ~Movement();
    GOLEM_ENGINE_API void Update() override;

    float moveSpeed = 1.0f;
    float rotateSpeed = 5;

    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 worldUp;

    GameObject* flowTarget;

    bool moveMode = false;

    GOLEM_ENGINE_API void ToJson(json& j) const
    {
        j = json
        {
            
        };
    }
};

REFL_AUTO(
    type(Movement),
    field(moveSpeed),
    field(moveMode)
)