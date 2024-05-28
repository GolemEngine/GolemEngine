#pragma once

#include "dll.h"

#include <unordered_map>
#include <functional>

#include "displayType.h"
#include "Resource/sceneManager.h"
#include "Components/Physic/boxCollider.h"
#include "Components/Physic/sphereCollider.h"
#include "Components/Physic/capsuleCollider.h"
#include "Components/Physic/meshCollider.h"

class ClassesManager
{
private:
	struct ClassInfo
	{
		std::function<void*(void)> create;
		std::function<void(void*)> display;
		std::function<bool(GameObject*)> createCondition;
		std::string name;
		size_t hashCode;
	};

	template <typename T>
	struct CreateCondition
	{
		_NODISCARD static bool CanCreate(GameObject*) { return true; }
	};

	template <>
	struct CreateCondition<Transform>
	{
		_NODISCARD static bool CanCreate(GameObject*) { return false; }
	};

	template <>
	struct CreateCondition<DirectionalLight>
	{
		_NODISCARD static bool CanCreate(GameObject*) { return SceneManager::GetCurrentScene()->GetDirectionalLights().size() < SceneManager::GetCurrentScene()->GetMaxDirectionalLights(); }
	};

	template <>
	struct CreateCondition<PointLight>
	{
		_NODISCARD static bool CanCreate(GameObject*) { return SceneManager::GetCurrentScene()->GetPointLights().size() < SceneManager::GetCurrentScene()->GetMaxPointLights(); }
	};

	template <>
	struct CreateCondition<SpotLight>
	{
		_NODISCARD static bool CanCreate(GameObject*) { return SceneManager::GetCurrentScene()->GetSpotLights().size() < SceneManager::GetCurrentScene()->GetMaxSpotLights(); }
	};

	template <>
	struct CreateCondition<BoxCollider>
	{
		_NODISCARD static bool CanCreate(GameObject* _obj)
		{
			return !(_obj->HasComponent("SphereCollider") || _obj->HasComponent("MeshCollider") || _obj->HasComponent("CapsuleCollider"));
		}
	};

	template <>
	struct CreateCondition<SphereCollider>
	{
		_NODISCARD static bool CanCreate(GameObject* _obj)
		{
			return !(_obj->HasComponent("BoxCollider") || _obj->HasComponent("MeshCollider") || _obj->HasComponent("CapsuleCollider"));
		}
	};

	template <>
	struct CreateCondition<MeshCollider>
	{
		_NODISCARD static bool CanCreate(GameObject* _obj)
		{
			return !(_obj->HasComponent("BoxCollider") || _obj->HasComponent("SphereCollider") || _obj->HasComponent("CapsuleCollider"));
		}
	};

	template <>
	struct CreateCondition<CapsuleCollider>
	{
		_NODISCARD static bool CanCreate(GameObject* _obj)
		{
			return !(_obj->HasComponent("BoxCollider") || _obj->HasComponent("SphereCollider") || _obj->HasComponent("MeshCollider"));
		}
	};

	static inline std::unordered_map<size_t, ClassInfo> m_classes;
	static inline std::unordered_map<std::string, ClassInfo> m_classesByName;

public:
	template<typename TypeT>
	static void Add();
	GOLEM_ENGINE_API static void AddAllClasses(); // Should modify this function if a new component class appears
	GOLEM_ENGINE_API static void Display(size_t _hashCode, void* _object);
	GOLEM_ENGINE_API static void* Create(size_t _hashCode);
	GOLEM_ENGINE_API static void* Create(const std::string& _name, GameObject* _obj);
	GOLEM_ENGINE_API static std::vector<std::string> GetComponents();
	GOLEM_ENGINE_API _NODISCARD static size_t GetHashCodeFromName(const std::string& _name);
};

template<typename TypeT>
void ClassesManager::Add()
{
	// Get class hashcode
	size_t hashCode = typeid(TypeT).hash_code();

	// Associate a DisplayField function to the class represented by hash_code 
	ClassInfo classInfo = {
		.create = []() -> void* { return nullptr; },
		.display = [](void* _class) -> void { DisplayType::DisplayField<TypeT>(static_cast<TypeT*>(_class)); },
		.createCondition = CreateCondition<TypeT>::CanCreate,
		.name = refl::reflect<TypeT>().name.c_str(),
		.hashCode = hashCode
	};

	// Check the abstract class case
	if constexpr (!std::is_abstract_v<TypeT>)
		classInfo.create = []() -> void* { return new TypeT; };

	// Store values in std::unordered_map
	m_classes.emplace(hashCode, classInfo);
	m_classesByName.emplace(classInfo.name, classInfo);
}

