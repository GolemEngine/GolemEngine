#include "UI/Windows/debugWindow.h"

#include "golemEngine.h"
#include "Inputs/inputManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Resource/sceneManager.h"


DebugWindow::DebugWindow(std::string _name)
	: Window(_name)
{}

DebugWindow::~DebugWindow() {}

void DebugWindow::Update()
{
	ImGui::Begin(name.c_str());
	ImGui::Text("deltaTime: %f", GolemEngine::GetDeltaTime());
	ImGui::Text("time: %f", GolemEngine::GetTime());
	ImGui::Text("fps: %.0f", 1.0f / GolemEngine::GetDeltaTime());
	ImGui::Text("Screen size: %.0f %.0f", WindowWrapper::GetScreenSize().x, WindowWrapper::GetScreenSize().y);
	ImGui::Text("Mouse position relative to viewport: %.0f %.0f", GolemEngine::mouseViewportPos.x, GolemEngine::mouseViewportPos.y);
	ImGui::Text("Mouse position relative to screen: %.0f %.0f", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
	ImGui::Text("Camera position: %.2f %.2f %.2f", GolemEngine::GetCamera()->position.x, GolemEngine::GetCamera()->position.y, GolemEngine::GetCamera()->position.z);
	DrawFpsGraph();
	ImGui::End();
}

void DebugWindow::DrawFpsGraph()
{
	static std::vector<float> fpsHistory;
	int maxHistorySize = 100;
	float currentFPS = 1.0f / GolemEngine::GetDeltaTime();
	fpsHistory.push_back(currentFPS);
    
	if (fpsHistory.size() > maxHistorySize) {
		fpsHistory.erase(fpsHistory.begin());
	}

	ImGui::PlotLines("FPS", fpsHistory.data(), (int)fpsHistory.size(), 0, "FPS", 0, 120, ImVec2(0, 80));
}