#include "Components/GameClasses/spawner.h"
#include "Core/scene.h"
#include "Core/gameobject.h"
#include "Resource/sceneManager.h"
#include "Resource/resourceManager.h"
#include "Components/meshRenderer.h"
#include "Components/component.h"
#include "Components/Physic/sphereCollider.h"
#include "Components/Physic/boxCollider.h"
#include "Components/GameClasses/enemy.h"
#include "Core/mesh.h"
#include "vector3.h"
#include <string>

#include "golemEngine.h"
Spawner::Spawner()
{
}

Spawner::~Spawner()
{
}

void Spawner::Begin()
{
}

void Spawner::Update()
{
	std::string name = "Enemy";
	// Using the rename functions
	int suffix = 2; // start at 2 because of two objects having the same name
	std::string originalName = name;
	while (SceneManager::GetCurrentScene()->IsNameExists(name))
	{
		name = originalName + "_" + std::to_string(suffix++);
	}

	if(GolemEngine::GetGameMode())
		interval -= GolemEngine::GetDeltaTime();

	if (interval <= 0 && spawnCount <= spawnLimit)
	{
		Spawn(name);
		interval = spawnInterval;
	}
}

void Spawner::Spawn(const std::string& _name)
{
	Enemy* enemy = new Enemy(owner->transform->localPosition, _name);
	spawnCount += 1;
}
