#pragma once

#include "Core/camera.h"
#include "UI/Windows/window.h"


class Camera;
extern bool g_isFromFileBrowser;

class Viewport : public Window
{
private:
	Camera* m_camera = nullptr;
	double m_cursorX;
	double m_cursorY;
	float m_lastX;
	float m_lastY;
	float m_yaw;
	float m_pitch;
	float m_roll;
	bool m_firstMouse;
	bool m_isDragging = false;
	bool m_lastSpacePress = false;
	Vector2 m_viewportBounds[2];

public:
	int width = 0, height = 0;

public:

	Viewport(std::string _name);
	~Viewport();

	void Update() override;
	void SetCamera(Camera* _camera);
	void DragDropModel();

	Vector2 GetViewportSize();
	Camera* GetCamera();
};