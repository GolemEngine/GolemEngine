#include "UI/Windows/sceneGraph.h"

#include "golemEngine.h"
#include "UI/editorUi.h"
#include "Core/gameobject.h"
#include "Components/transform.h"
#include "Resource/sceneManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"


SceneGraph::SceneGraph(std::string _name) 
	: Window(_name)
{}

SceneGraph::~SceneGraph() {}

void SceneGraph::Update()
{
	ImGui::Begin(name.c_str());
	ImGui::Text("%s", SceneManager::GetCurrentScene()->name.c_str());
	DisplayObjects(SceneManager::GetCurrentScene()->GetWorld());

	ImGui::End();
}

void SceneGraph::DisplayObjects(GameObject* _gameObject)
{
	const std::vector<Transform*>& children = _gameObject->transform->GetChildren();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
	
	if (children.size() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	
	if (_gameObject == EditorUi::selectedGameObject)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	
	std::string n = _gameObject->name;
	const char* name = n.c_str();
	size_t id = _gameObject->GetId();

	if (m_renamingGameObject == _gameObject)
	{
		name = "##input";
	}
	
	if (ImGui::TreeNodeEx(name, flags) && _gameObject)
	{
		if (m_renamingGameObject == _gameObject)
		{
			ImGui::SameLine();
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText(name, &_gameObject->name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
			{
				m_renamingGameObject = nullptr;
			}
		}
		else
		{
			// Rename popup
			if (ImGui::BeginPopupContextItem(std::to_string(id).c_str()))
			{
				if (ImGui::MenuItem("Rename"))
				{
					m_renamingGameObject = _gameObject;
				}
				ImGui::EndPopup();
			}
	
			// Create popup
			if (ImGui::BeginPopupContextItem(std::to_string(id).c_str()))
			{
				if (ImGui::MenuItem("Create"))
				{
					new GameObject("New GameObject", new Transform(_gameObject->transform));
				}
				ImGui::EndPopup();
			}
	
			// Delete popup
			if (ImGui::BeginPopupContextItem(std::to_string(id).c_str()))
			{
				if (ImGui::MenuItem("Delete") && _gameObject != SceneManager::GetCurrentScene()->GetWorld())
				{
					if (_gameObject == EditorUi::selectedGameObject)
					{
						EditorUi::selectedGameObject->IsSelected = false;
						EditorUi::selectedGameObject = nullptr;
					}
					delete _gameObject;
				}
				ImGui::EndPopup();
			}
	
			// Drag and Drop management
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("Golem", &_gameObject, sizeof(_gameObject));
	
				ImGui::Text("%s", name); 
				ImGui::EndDragDropSource();
			}
	
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* dragged = ImGui::AcceptDragDropPayload("Golem");
	
				if (dragged)
				{
					GameObject* gameObjectDragged = *(GameObject**)dragged->Data;
	
					if (!gameObjectDragged->transform->IsAParentOf(_gameObject->transform) && gameObjectDragged != SceneManager::GetCurrentScene()->GetWorld())
					{
						gameObjectDragged->transform->SetParent(_gameObject->transform);
					}
				}
	
				ImGui::EndDragDropTarget();
			}
	
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				if (EditorUi::selectedGameObject)
					EditorUi::selectedGameObject->IsSelected = false;
				EditorUi::selectedGameObject = _gameObject;
				EditorUi::selectedGameObject->IsSelected = true;
			}
		}
	
		for (Transform* transform : children)
		{
			DisplayObjects(transform->owner);
		}
	
		ImGui::TreePop();
	}
	else
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			EditorUi::selectedGameObject = _gameObject;
		}
	}
}


