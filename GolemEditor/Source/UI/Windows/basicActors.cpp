#include "UI/Windows/basicActorsWindow.h"

#include "Core/gameobject.h"
#include "golemEngine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Refl/refl.hpp"

BasicActorsWindow::BasicActorsWindow(std::string _name) 
	: Window(_name)
{}

BasicActorsWindow::~BasicActorsWindow() {}

void BasicActorsWindow::Update()
{
	ImGui::Begin(name.c_str());

	ImGui::End();
}