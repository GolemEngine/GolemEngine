#include "Components/GameClasses/playerCamera.h"

#include "golemEngine.h"

PlayerCamera::PlayerCamera()
{
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::Begin()
{
}

void PlayerCamera::Update()
{
	GolemEngine::GetPlayerCamera()->position = owner->transform->globalPosition;
}