#pragma once

#include <vector>
#include <string>

#include "Core/gameobject.h"
#include "WorldBuilder/terrain.h"
#include "UI/toolbar.h"
#include "vector2.h"


class Window;

class EditorUi
{
private:
	static inline std::vector<Window*> m_windows;
	static inline Toolbar* m_toolbar;

public:
	static inline GameObject* selectedGameObject = nullptr;

public:
	static void Init();
	static void SetColorStyles();
	static void BeginDockSpace();
	static void EndDockSpace();
	static void UpdateWindows();

	static Window* GetWindowByName(std::string _name);
	static Vector2 GetDockedWindowPosition(std::string _dockedWindowName);
};