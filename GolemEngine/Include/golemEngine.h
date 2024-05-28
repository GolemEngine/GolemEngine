#pragma once

#include <string>
#include <vector>

#include "dll.h"
#include "Core/scene.h"
#include "Physic/physicSystem.h"


struct GLFWwindow;
class Camera;

class GOLEM_ENGINE_API GolemEngine
{
private:
	static inline PhysicSystem* m_physicSystem = nullptr;
	static inline Camera* m_camera = nullptr;
	static inline Camera* m_playerCamera = nullptr;
	static inline float m_deltaTime = 0.0f;
	static inline bool m_gameMode = false;

public:
	static inline GameObject* selectedGameObject = nullptr;
	static inline Vector2 mouseViewportPos = {0.0f, 0.0f};

private:
	// Private constructors and destructor to make static class
	GolemEngine() = delete;
	GolemEngine(GolemEngine& _other) = delete;
	GolemEngine(GolemEngine&& _other) = delete;
	GolemEngine& operator=(GolemEngine& _other) = delete;
	GolemEngine& operator=(GolemEngine&& _other) = delete;
	void operator=(const GolemEngine&) = delete;
	~GolemEngine() = delete;

	static void PerformComputShaderComputations();

public:
	static void Init();
	static void UpdateDeltaTime();
	static void ProcessInput();
	static void Update();
	static void Close();

	static Camera* GetCamera();
	static Camera* GetPlayerCamera();
	static float GetDeltaTime();
	static float GetTime();
	static bool GetGameMode();
	static void StartGameMode();
	static void StopGameMode();
};