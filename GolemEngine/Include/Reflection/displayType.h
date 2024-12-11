#pragma once

#include <vector>
#include <iostream>

#include "vector3.h"
#include "vector4.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "Reflection/attributes.h"
#include "Refl/refl.hpp"
#include "Core/gameObject.h"
#include "MagicEnum/magic_enum.hpp"
#include "vector2.h"

class DisplayType
{
private:
	static const char* m_addComponentPopupId;
	static const char* m_addComponentButtonName;
	static Vector2 m_addComponentButtonSize;

public:
	template<typename TypeT>
	static void DisplayField(TypeT* _class);

	template<typename TypeT, typename MemberT, typename DescriptorT>
	static void BasicsFields(MemberT* _class);

	template<typename TypeT, typename MemberT, typename DescriptorT>
	static void DisplayStdVector(MemberT* _class);

	template<typename TypeT, typename MemberT, typename DescriptorT>
	static void DisplayEnum(MemberT* _class);

	template<typename TypeT, typename MemberT, typename DescriptorT>
	static void DisplayIntOrFloat(MemberT* _member);

	template<typename TypeT>
	static void RemoveComponentButton(TypeT* _class);
	GOLEM_ENGINE_API static void DisplayWithHashCode(size_t _hashCode, void* _object);
	GOLEM_ENGINE_API static void AddComponentHandler(GameObject* _gameObject);
};

template<typename>
struct is_std_vector : std::false_type {};

template<typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type {};

template<typename T>
constexpr bool is_std_vector_v = is_std_vector<T>::value;

template<typename TypeT>
void DisplayType::DisplayField(TypeT* _class)
{
	constexpr auto type = refl::reflect<TypeT>();	// Get the reflected class
	RemoveComponentButton(_class);
	for_each(type.members, [&]<typename DescriptorT>(const DescriptorT)	// Loop through each member of the reflected class
	{
		using MemberT = DescriptorT::value_type;

		if constexpr (!refl::descriptor::has_attribute<HideInInspector>(DescriptorT{}))
		{
			if constexpr (is_std_vector_v<MemberT>) // std::vector case
			{
				DisplayStdVector<TypeT, MemberT, DescriptorT>(&DescriptorT::get(_class));
			}
			else if constexpr (std::is_enum_v<MemberT>) // enum case
			{
				DisplayEnum<TypeT, MemberT, DescriptorT>(&DescriptorT::get(_class));
			}
			else // basic case
			{
				BasicsFields<TypeT, MemberT, DescriptorT>(&DescriptorT::get(_class));
			}
		}
	});
}

template<typename TypeT, typename MemberT, typename DescriptorT>
void DisplayType::BasicsFields(MemberT* _class)
{
	ImGui::PushID(_class);
	if constexpr (std::is_pointer_v<MemberT>)
	{
		DisplayWithHashCode(typeid(** _class).hash_code(), *_class);
	}
	// Display Imgui with all original types
	else if constexpr (std::is_same_v<std::string, MemberT>) // string
	{
		ImGui::InputText(DescriptorT::name.c_str(), _class);
	}
	else if constexpr (std::is_same_v<bool, MemberT>) // bool
	{
		ImGui::Checkbox(DescriptorT::name.c_str(), _class);
	}
	else if (std::is_integral_v<MemberT> || std::is_floating_point_v<MemberT>)
	{
		DisplayIntOrFloat<TypeT, MemberT, DescriptorT>(_class);
	}
	else if constexpr (std::is_same_v<Vector3, MemberT>)
	{
		if constexpr (refl::descriptor::has_attribute<Range>(DescriptorT{}))
		{
			auto& range = refl::descriptor::get_attribute<Range>(DescriptorT{});
			ImGui::DragScalarN(DescriptorT::name.c_str(), ImGuiDataType_Float, &_class->x, 3, 0.1f, &range.min, &range.max, nullptr, ImGuiSliderFlags_AlwaysClamp);
		}
		else
			ImGui::DragFloat3(DescriptorT::name.c_str(), &_class->x, 0.1f);
	}
	else if constexpr (std::is_same_v<Vector4, MemberT>)
	{
		if constexpr (refl::descriptor::has_attribute<Range>(DescriptorT{}))
		{
			auto& range = refl::descriptor::get_attribute<Range>(DescriptorT{});
			ImGui::DragScalarN(DescriptorT::name.c_str(), ImGuiDataType_Float, &_class->x, 4, 0.1f, nullptr, &range.min, &range.max, nullptr, ImGuiSliderFlags_AlwaysClamp);
		}
		else
			ImGui::DragFloat4(DescriptorT::name.c_str(), &_class->x, 0.1f);
	}
	ImGui::PopID();
}

template<typename TypeT, typename MemberT, typename DescriptorT>
void DisplayType::DisplayStdVector(MemberT* _class)
{
	for (unsigned int i = 0; i < _class->size(); i++)
	{
		BasicsFields<TypeT, typename MemberT::value_type, DescriptorT>(&(*_class)[i]);
	}
}

template<typename TypeT, typename MemberT, typename DescriptorT>
void DisplayType::DisplayEnum(MemberT* _class)
{
	constexpr auto enumName = magic_enum::enum_type_name<MemberT>();

	auto name = magic_enum::enum_name(*_class);
	std::string nameStr(name);
	if (ImGui::BeginCombo(enumName.data(), nameStr.c_str()))
	{
		for (auto name : magic_enum::enum_names<MemberT>())
		{
			std::string nameStr(name);
			if (ImGui::Selectable(nameStr.c_str(), _class))
			{
				*_class = magic_enum::enum_cast<MemberT>(name).value_or(*_class);
			}
		}
		ImGui::EndCombo();
	}
}

template<typename TypeT, typename MemberT, typename DescriptorT>
void DisplayType::DisplayIntOrFloat(MemberT* _member)
{
	int type = ImGuiDataType_S8;
	if constexpr (std::is_same_v<std::int8_t, MemberT>) // int_8
	{
		type = ImGuiDataType_S8;
	}
	else if constexpr (std::is_same_v<std::int16_t, MemberT>) // int_16
	{
		type = ImGuiDataType_S16;
	}
	else if constexpr (std::is_same_v<std::int32_t, MemberT>) // int_32
	{
		type = ImGuiDataType_S32;
	}
	else if constexpr (std::is_same_v<std::uint8_t, MemberT>) // uint_8
	{
		type = ImGuiDataType_U8;
	}
	else if constexpr (std::is_same_v<std::uint16_t, MemberT>) // uint_16
	{
		type = ImGuiDataType_U16;
	}
	else if constexpr (std::is_same_v<std::uint32_t, MemberT>) // uint_32
	{
		type = ImGuiDataType_U32;
	}
	else if constexpr (std::is_same_v<float, MemberT>) // float 
	{
		type = ImGuiDataType_Float;
	}
	else if constexpr (std::is_same_v<double, MemberT>) // double
	{
		type = ImGuiDataType_Double;
	}

	ImGui::PushID(_member);

	if constexpr (refl::descriptor::has_attribute<Range>(DescriptorT{}))
	{
			auto& range = refl::descriptor::get_attribute<Range>(DescriptorT{});
			ImGui::DragScalar(DescriptorT::name.c_str(), type, _member, 0.1f, &range.min, &range.max, nullptr, ImGuiSliderFlags_AlwaysClamp);
	}
	else
		ImGui::DragScalar(DescriptorT::name.c_str(), type, _member, 0.1f);

	ImGui::PopID();
}

template<typename TypeT>
void DisplayType::RemoveComponentButton(TypeT* _class)
{
	constexpr auto type = refl::reflect<TypeT>();	// Get the reflected class
	Component* c = dynamic_cast<Component*>(_class);
	if (c && !dynamic_cast<Transform*>(_class))  // If class is component replace the name by a button that can delete it (can't delete transform)
	{
		const char* removeComponentPopupId = type.name.c_str();

		if (ImGui::Button(removeComponentPopupId))
		{
			ImGui::OpenPopup(removeComponentPopupId);
		}
		if (ImGui::BeginPopupContextItem(removeComponentPopupId))
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				c->owner->RemoveComponent(c);
				ImGui::EndPopup();
				return;
			}
			ImGui::EndPopup();
		}
	}
	else
	{
		ImGui::Text("%s", type.name.c_str());
	}
}