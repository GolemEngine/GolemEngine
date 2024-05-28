#pragma once

#include <string>
#include <vector>
#include <type_traits>
#include <nlohmann/json.hpp>

#include "dll.h"
#include "Refl/refl.hpp"
#include "Components/transform.h"
#include "Resource/guid.h"
#include "Debug/log.h"
#include "Components/Light/light.h"

using json = nlohmann::json;


class GameObject
{
private:
	size_t m_id;
	std::vector<Component*> m_components;

public:
	Guid guid;
	std::string name;
	Transform* transform = nullptr;
	bool isTerrain = false;
	bool IsSelected;

public:
	GOLEM_ENGINE_API GameObject();
	GOLEM_ENGINE_API GameObject(const std::string& _name, Transform* _transform);
	GOLEM_ENGINE_API virtual ~GameObject();

	GOLEM_ENGINE_API void UpdateAllComponents();
	GOLEM_ENGINE_API void virtual Update();
	GOLEM_ENGINE_API _NODISCARD bool HasComponent(const std::string& _name) const;

	GOLEM_ENGINE_API size_t GetId();
	GOLEM_ENGINE_API void SetId(size_t _id);

	GOLEM_ENGINE_API void DeleteTransform(Transform* _t);
	GOLEM_ENGINE_API void DeleteLight(Light* _l);

	GOLEM_ENGINE_API void RemoveComponent(Component* _c);
	GOLEM_ENGINE_API void DeleteAllComponents();

	GOLEM_ENGINE_API void AddComponent(Component* _type);
	template<typename TypeT>
	void AddComponent(TypeT* _type);
	template<typename TypeT>
	void AddComponent();

	template<typename TypeT>
	TypeT* GetComponent(); 

	template<typename TypeT>
	std::vector<TypeT*> GetComponents();

	friend refl_impl::metadata::type_info__<GameObject>; // needed to reflect private members

	// Define serialization and deserialization functions manually because the
	// macro is not used due to the pointer member variable.
	GOLEM_ENGINE_API void ToJson(json& _j) const;
};

template<typename TypeT>
void GameObject::AddComponent(TypeT* _type)
{
	if (GetComponent<TypeT>())
	{
		Log::Print("The GameObject already has a Component of type TypeT");
		return;
	}
	static_assert(std::is_base_of_v<Component, TypeT>, "TypeT isn't a component");
	m_components.push_back(_type);
	_type->owner = this;
}

template<typename TypeT>
void GameObject::AddComponent()
{
	static_assert(std::is_base_of_v<Component, TypeT>, "TypeT isn't a component");
	TypeT* t = new TypeT;
	m_components.push_back(t);
	t->owner = this;
}

template<typename TypeT>
TypeT* GameObject::GetComponent()
{
	for (Component* c : m_components)
	{
		auto t = dynamic_cast<TypeT*>(c);
		if (t)
		{
			return t;
		}
	}
	return nullptr;
}

template<typename TypeT>
std::vector<TypeT*> GameObject::GetComponents()
{
	std::vector<TypeT*> components;
	for (Component* c : m_components)
	{
		auto t = dynamic_cast<TypeT*>(c);
		if (t)
		{
			components.push_back(t);
		}
	}
	return components;
}

REFL_AUTO(
	type(GameObject),
	field(name),
	field(m_components)
)