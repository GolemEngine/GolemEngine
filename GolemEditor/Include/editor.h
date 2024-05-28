#pragma once

#include "Core/scene.h"

class Editor
{
private:
	std::string m_name;

public:
	Editor();
	~Editor();

	void InitWindow();
	void InitGraphics();
	void InitUi();
	void Init();
	void Run();
	void MainLoop();
	void Cleanup();
};