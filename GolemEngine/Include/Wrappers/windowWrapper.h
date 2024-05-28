#pragma once

#include "dll.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vector2.h"

#pragma region Keyboard defines
#define LAST_KEY_INT GLFW_KEY_LAST    // To get last keybaord key to define size of array of all keybard keys
#define KEY_PRESSED GLFW_PRESS
#define KEY_RELEASED GLFW_RELEASE

#define KEY_ESCAPE GLFW_KEY_ESCAPE
#define KEY_SPACE GLFW_KEY_SPACE
#define KEY_A GLFW_KEY_A
#define KEY_B GLFW_KEY_B
#define KEY_C GLFW_KEY_C
#define KEY_D GLFW_KEY_D
#define KEY_E GLFW_KEY_E
#define KEY_F GLFW_KEY_F
#define KEY_G GLFW_KEY_G
#define KEY_H GLFW_KEY_H
#define KEY_I GLFW_KEY_I
#define KEY_J GLFW_KEY_J
#define KEY_K GLFW_KEY_K
#define KEY_L GLFW_KEY_L
#define KEY_M GLFW_KEY_M
#define KEY_N GLFW_KEY_N
#define KEY_O GLFW_KEY_O
#define KEY_P GLFW_KEY_P
#define KEY_Q GLFW_KEY_Q
#define KEY_R GLFW_KEY_R
#define KEY_S GLFW_KEY_S
#define KEY_T GLFW_KEY_T
#define KEY_U GLFW_KEY_U
#define KEY_V GLFW_KEY_V
#define KEY_W GLFW_KEY_W
#define KEY_X GLFW_KEY_X
#define KEY_Y GLFW_KEY_Y
#define KEY_Z GLFW_KEY_Z
#define KEY_LEFT_CTRL GLFW_KEY_LEFT_CONTROL
#pragma endregion Keyboard defines

#pragma region Mouse defines
#define LAST_MOUSE_BUTTON_INT GLFW_MOUSE_BUTTON_LAST    // To get last mouse button to define size of array of all mouse buttons
#define BUTTON_PRESSED GLFW_PRESS
#define BUTTON_RELEASED GLFW_RELEASE

#define BUTTON_0 GLFW_MOUSE_BUTTON_1
#define BUTTON_1 GLFW_MOUSE_BUTTON_2
#define	BUTTON_2 GLFW_KEY_V
#pragma endregion Mouse defines

#pragma region Other defines
#define INPUT_CURSOR GLFW_CURSOR
#define OPTION_CURSOR_DISABLED GLFW_CURSOR_DISABLED
#pragma endregion Other defines


using Golemint = GLuint;

class GOLEM_ENGINE_API WindowWrapper
{
public:
	static GLFWwindow* window;
	static Vector2 screenSize;

private:
	// Private constructors and destructor to make static class
	WindowWrapper() = delete;
	WindowWrapper(WindowWrapper& _other) = delete;
	WindowWrapper(WindowWrapper&& _other) = delete;
	WindowWrapper& operator=(WindowWrapper& _other) = delete;
	WindowWrapper& operator=(WindowWrapper&& _other) = delete;
	void operator=(const WindowWrapper&) = delete;
	~WindowWrapper() = delete;

public:
	// Init GLFW
	static void InitWindow();
	// Create new GLFW window
	static GLFWwindow* NewWindow(int _width, int _height, const char* _title, GLFWmonitor* _monitor, GLFWwindow* _share);
	// Terminate in case of error
	static void Terminate();
	// Check if user tried to close window
	static int ShouldWindowClose(GLFWwindow* _window);
	// Process different events to the window
	static void ProcessEvents();
	// Swap between front and back buffer to render (back) or display (front)
	static void SwapBuffers(GLFWwindow* _window);
	// Makes the window passed in parameter the window that will be set as active (to perform operations on...etc).
	static void MakeContext(GLFWwindow* _window);
	static GLuint LoadUiTexture(const char* _filename);

	static Vector2 GetScreenSize();
	
	static void SetScreenSize(Vector2 _screenSize);
};