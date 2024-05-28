#include "Reflection/classesManager.h"
#include "Debug/log.h"
#include "Components/transform.h"
#include "Components/meshRenderer.h"
#include "Components/Light/directionalLight.h"
#include "Components/Light/pointLight.h"
#include "Components/Light/spotLight.h"
#include "Components/audio.h"
#include "Components/Physic/sphereCollider.h"
#include "Components/Physic/boxCollider.h"
#include "Components/GameClasses/movement.h"
#include "WorldBuilder/terrain.h"
#include "Components/Physic/meshCollider.h"
#include "Components/Physic/capsuleCollider.h"
#include "Components/GameClasses/playerMovement.h"
#include "Components/GameClasses/playerCamera.h"
#include "Components/GameClasses/spawner.h"

void ClassesManager::AddAllClasses()
{
	Add<Component>();
	Add<Transform>();
	Add<MeshRenderer>();
	Add<DirectionalLight>();
	Add<PointLight>();
	Add<SpotLight>();
	Add<Audio>();
	Add<SphereCollider>();
	Add<BoxCollider>();
	Add<MeshCollider>();
	Add<CapsuleCollider>();
	Add<Movement>();
	Add<PlayerMovement>();
	Add<PlayerCamera>();
	Add<Spawner>();
}

void ClassesManager::Display(size_t _hashCode, void* _object)
{
	auto it = m_classes.find(_hashCode);

	// return if no classes linked to hash code
	if (it == m_classes.end())
	{
		Log::Print("No class matches the hash code");
		return;
	}
	// else store it in display func pointer
	it->second.display(_object);
}

void* ClassesManager::Create(size_t _hashCode)
{
	auto it = m_classes.find(_hashCode);

	// return if no classes linked to hash code
	if (it == m_classes.end())
	{
		Log::Print("No class matches the hash code");
		return nullptr;
	}

	if (!it->second.createCondition(nullptr))
		return nullptr;

	// else store it in display func pointer
	return it->second.create();
}

void* ClassesManager::Create(const std::string& _name, GameObject* _obj)
{
	auto it = m_classesByName.find(_name);

	// return if no classes linked to hash code
	if (it == m_classesByName.end())
	{
		Log::Print("No class matches the name");
		return nullptr;
	}

	if (!it->second.createCondition(_obj))
		return nullptr;

	// else store it in display func pointer
	return it->second.create();
}

std::vector<std::string> ClassesManager::GetComponents()
{
	std::vector<std::string> result;
	for (const std::string& name : m_classesByName | std::views::keys)
	{
		result.push_back(name);
	}
	return result;
}

size_t ClassesManager::GetHashCodeFromName(const std::string& _name)
{
	auto it = m_classesByName.find(_name);

	// return if no classes linked to name
	if (it == m_classesByName.end())
	{
		Log::Print("No class matches the name");
		return 0;
	}

	return it->second.hashCode;
}
