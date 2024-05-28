#pragma once
#include "UI/Windows/window.h"
#include "vector2.h"

extern bool g_isPlayTesting;

class PlayScene : public Window
{
private:
	Vector2 m_lastMousepos = { 0.0f, 0.0f };
public:
	int width = 0, height = 0;


	PlayScene(std::string _name);
	~PlayScene();

	void Update() override;
};