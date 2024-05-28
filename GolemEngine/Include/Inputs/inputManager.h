#pragma once

#include "dll.h"
#include "Wrappers/windowWrapper.h"
#include "vector2.h"


class Camera;

class GOLEM_ENGINE_API InputManager
{
private:
#pragma region Keyboard variables
	static inline bool m_keyboardKeys[LAST_KEY_INT];
#pragma endregion Keyboard variables

#pragma region Mouse variables
	static Vector2 m_mouseWindowPos;
	static float m_mouseScrollValue;
	static inline int m_mouseButtons[LAST_MOUSE_BUTTON_INT];
#pragma endregion Mouse variables

private:
	// Private constructors and destructor to make static class
	InputManager() = delete;
	InputManager(InputManager& _other) = delete;
	InputManager(InputManager&& _other) = delete;
	InputManager& operator=(InputManager& _other) = delete;
	InputManager& operator=(InputManager&& _other) = delete;
	void operator=(const InputManager&) = delete;
	~InputManager() = delete;

public:
	// Set callbacks application will use
	static void Init(GLFWwindow* _window);
	// Set custom keyboard callback
	static void KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
	// Set custom mouse movement callback
	static void MousePositionCallback(GLFWwindow* _window, double _xPos, double _yPos);
	// Set custom mouse button click callback
	static void MouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods);
	// Set custom mouse scroll callback
	static void MouseScrollCallback(GLFWwindow* _window, double _xOffset, double _yOffset);
	// Check if key (see WindowWrapper defines) is pressed
	static bool IsKeyPressed(int _key);
	// Check if button (see WindowWrapper defines) is pressed
	static bool IsButtonPressed(int _button);
	// Check if button (see WindowWrapper defines) is released
	static bool IsButtonReleased(int _button);

	// Get mouse position relative to glfw window
	static Vector2 GetMouseWindowPos();
	static float GetMouseScroll();

	static void SetMouseScroll(float _value);
};