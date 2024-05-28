#include "Components/GameClasses/movement.h"
#include <iostream>
#include "Core/gameobject.h"
#include "golemEngine.h"
#include "Inputs/inputManager.h"
#include "utils.h"

Movement::Movement()
    :front(0.0f, 0.0f, -1.0f),
    worldUp(0.0f, 1.0f, 0.0f)
{
}

Movement::~Movement()
{
}

void Movement::Update()
{
	if (owner && GolemEngine::GetGameMode())
	{
        if (!moveMode)
        {
            if (InputManager::IsKeyPressed(KEY_W))
            {
                owner->transform->localPosition += front * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_S))
            {
                owner->transform->localPosition -= front * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_A))
            {
                owner->transform->localPosition.x -= moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_D))
            {
                owner->transform->localPosition.x += moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_Q))
            {
                owner->transform->rotation.y -= moveSpeed * GolemEngine::GetDeltaTime() * 10;
            }
            if (InputManager::IsKeyPressed(KEY_E))
            {
                owner->transform->rotation.y += moveSpeed * GolemEngine::GetDeltaTime() * 10;
            }
        }
        
        //owner->transform->localPosition = GolemEngine::GetPlayerCamera()->position;

        if (moveMode)
        {
            if (InputManager::IsKeyPressed(KEY_W))
            {
                GolemEngine::GetPlayerCamera()->position += GolemEngine::GetPlayerCamera()->GetFront() * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_S))
            {
                GolemEngine::GetPlayerCamera()->position -= GolemEngine::GetPlayerCamera()->GetFront() * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_A))
            {
                GolemEngine::GetPlayerCamera()->position -= GolemEngine::GetPlayerCamera()->GetRight() * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_D))
            {
                GolemEngine::GetPlayerCamera()->position += GolemEngine::GetPlayerCamera()->GetRight() * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_Q))
            {
                GolemEngine::GetPlayerCamera()->position -= GolemEngine::GetPlayerCamera()->GetUp() * moveSpeed * GolemEngine::GetDeltaTime();
            }
            if (InputManager::IsKeyPressed(KEY_E))
            {
                GolemEngine::GetPlayerCamera()->position += GolemEngine::GetPlayerCamera()->GetUp() * moveSpeed * GolemEngine::GetDeltaTime();
            }
        }
        
	}
}