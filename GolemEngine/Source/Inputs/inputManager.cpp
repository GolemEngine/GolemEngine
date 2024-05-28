#include "Inputs/inputManager.h"

#include <iostream>

#include "imgui.h"
#include "Core/camera.h"
#include "imgui/backends/imgui_impl_glfw.h"


Vector2 InputManager::m_mouseWindowPos = { 0.0f, 0.0f };
float InputManager::m_mouseScrollValue = 0.0f;

void InputManager::Init(GLFWwindow* _window)
{
    glfwSetKeyCallback(_window, KeyCallback);
    glfwSetCursorPosCallback(_window, MousePositionCallback);
    glfwSetScrollCallback(_window, MouseScrollCallback);
    glfwSetMouseButtonCallback(_window, MouseButtonCallback);
}

void InputManager::KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
    ImGui_ImplGlfw_KeyCallback(_window, _key, _scancode, _action, _mods);
    if (_action == KEY_PRESSED)
    {
        m_keyboardKeys[_key] = true;
    }
    else if (_action == KEY_RELEASED)
    {
        if (m_keyboardKeys[_key] == true)
        {
            m_keyboardKeys[_key] = false;
        }
    }
}

void InputManager::MousePositionCallback(GLFWwindow* _window, double _xPos, double _yPos)
{
    ImGui_ImplGlfw_CursorPosCallback(_window, _xPos, _yPos);
    m_mouseWindowPos = { (float)_xPos, (float)_yPos };
}

void InputManager::MouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(_window, _button, _action, _mods);
    if (_action == BUTTON_PRESSED)
    {
        m_mouseButtons[_button] = true;
    }
    else if (m_mouseButtons[_button] && _action == BUTTON_RELEASED)
    {
        {
            m_mouseButtons[_button] = false;
        }
    }
}

void InputManager::MouseScrollCallback(GLFWwindow* _window, double _xOffset, double _yOffset)
{
    ImGui_ImplGlfw_ScrollCallback(_window, _xOffset, _yOffset);
    m_mouseScrollValue = (float)_yOffset;
}

bool InputManager::IsKeyPressed(int _key)
{
    return m_keyboardKeys[_key];
}

bool InputManager::IsButtonPressed(int _button)
{
    return m_mouseButtons[_button];
}

bool InputManager::IsButtonReleased(int _button)
{
    return !m_mouseButtons[_button];
}

Vector2 InputManager::GetMouseWindowPos()
{
    return m_mouseWindowPos;
}

float InputManager::GetMouseScroll()
{
    return m_mouseScrollValue;
}

void InputManager::SetMouseScroll(float _value)
{
    m_mouseScrollValue = _value;
}