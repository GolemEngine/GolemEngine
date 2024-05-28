#include "Core/camera.h"

#include <Windows.h>

#include "Wrappers/windowWrapper.h" 
#include "Inputs/inputManager.h"
#include "utils.h"
#include "Debug/log.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui.h"


const int SCREEN_SIZE_LEFT_X_BORDER_MARGIN = 20;
const int SCREEN_SIZE_RIGHT_X_BORDER_MARGIN = 20;
const int TP_LEFT_X_POS_MARGIN = 20;
const int TP_LEFT_X_RIGHT_MARGIN = 20;
const int SCREEN_SIZE_TOP_Y_BORDER_MARGIN = 60;
const int SCREEN_SIZE_BOTTOM_Y_BORDER_MARGIN = 20;
const int TP_TOP_Y_POS_MARGIN = 80;
const int TP_LEFT_Y_BOTTOM_MARGIN = 20;

Camera::Camera(Vector3 _position, Vector3 _up, float _yaw, float _pitch)
    : 
    position(_position),
    m_front(Vector3(0.0f, 0.0f, -1.0f)),
    m_worldUp(_up),
    m_yaw(_yaw),
    m_pitch(_pitch),
    m_movementSpeed(2.5f),
    m_mouseSensitivity(0.1f),
    m_zoom(45.0f),
    m_near(0.1f),
    m_far(1000.0f),
    m_minSpeed(1.0f),
    m_maxSpeed(100.0f)
{
	UpdateVectors();
}

void Camera::ProcessKeyboardInput(float _deltaTime)
{
    if (InputManager::IsKeyPressed(KEY_W))
    {
        position += m_front * m_movementSpeed * _deltaTime;
    }
    if (InputManager::IsKeyPressed(KEY_S))
    {
        position -= m_front * m_movementSpeed * _deltaTime;
    }
    if (InputManager::IsKeyPressed(KEY_A))
    {
        position -= m_right * m_movementSpeed * _deltaTime;
    }
    if (InputManager::IsKeyPressed(KEY_D))
    {
        position += m_right * m_movementSpeed * _deltaTime;
    }
    if (InputManager::IsKeyPressed(KEY_Q))
    {
        position -= m_up * m_movementSpeed * _deltaTime;
    }
    if (InputManager::IsKeyPressed(KEY_E))
    {
        position += m_up * m_movementSpeed * _deltaTime;
    }
}

void Camera::ProcessMouseMovement(Vector2 _mouseWindowPos, bool _constrainPitch, Vector4 _windowDimension, int _mousePosX, int _mousePosY)
{
    // Due to imgui docking branched being used, the glfw function to disable the cursor allowing to move the mouse in the viewport in inifite x directions doesn't seem to work
    // so we need to move the mouse ourselves. To do so, the variable _windowDimension contains the x pos of the imgui window (left side) and its width to determine the x pos of the right size,
    // and same with y pos on bottom and height to determine the y pos on at the top.

    // Check if mouse got teleported on last frame to avoid new teleportation
    if (m_isMouseTp)
    {
        m_lastMousePos = _mouseWindowPos;
        m_isMouseTp = false;
    }

    // Teleport mouse 
    if (_mousePosX <= _windowDimension.x + SCREEN_SIZE_LEFT_X_BORDER_MARGIN)   // Mouse exits from left of Viewport
    {
        SetCursorPos((int)(_windowDimension.x + _windowDimension.y - SCREEN_SIZE_RIGHT_X_BORDER_MARGIN), _mousePosY);
        m_lastMousePos = InputManager::GetMouseWindowPos();
        m_isMouseTp = true;
    }
    if (_mousePosX >= _windowDimension.x + _windowDimension.y - SCREEN_SIZE_RIGHT_X_BORDER_MARGIN)  // Mouse exits from right of Viewport
    {
        SetCursorPos((int)(_windowDimension.x + SCREEN_SIZE_LEFT_X_BORDER_MARGIN), _mousePosY);
        m_lastMousePos = InputManager::GetMouseWindowPos();
        m_isMouseTp = true;
    }
    if (_mousePosY <= _windowDimension.z + SCREEN_SIZE_TOP_Y_BORDER_MARGIN)   // Mouse exits from top of Viewport
    {
        SetCursorPos(_mousePosX, (int)(_windowDimension.z + _windowDimension.w - TP_LEFT_Y_BOTTOM_MARGIN));
        m_lastMousePos = InputManager::GetMouseWindowPos();
        m_isMouseTp = true;
    }
    if (_mousePosY >= _windowDimension.z + _windowDimension.w - SCREEN_SIZE_BOTTOM_Y_BORDER_MARGIN)  // Mouse exits from bottom of Viewport
    {
        SetCursorPos(_mousePosX, (int)(_windowDimension.z + TP_TOP_Y_POS_MARGIN));
        m_lastMousePos = InputManager::GetMouseWindowPos();
        m_isMouseTp = true;
    }
    
    // Transform mouse position into an offset
    if (isFirstMouse)   // Check if it's the first click on the viewport to not teleport the camera
    {
        m_lastMousePos = _mouseWindowPos;
        isFirstMouse = false;
    }

    Vector2 mouseOffset = _mouseWindowPos - m_lastMousePos;
    m_lastMousePos = _mouseWindowPos;

    // Use this offset to change camera vector values thus moving the camera
    float xOffset = mouseOffset.x;
    float yOffset = -mouseOffset.y;

    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (_constrainPitch) 
    {
        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0f;
        }
        if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }
    }

    UpdateVectors();
}

void Camera::UpdateVectors()
{
    m_front.x = cos(DegToRad(m_yaw)) * cos(DegToRad(m_pitch));
    m_front.y = sin(DegToRad(m_pitch));
    m_front.z = sin(DegToRad(m_yaw)) * cos(DegToRad(m_pitch));
    m_front = m_front.Normalize();
    m_right = Vector3::Cross(m_front, m_worldUp).Normalize();
    m_up = Vector3::Cross(m_right, m_front).Normalize();
}

void Camera::ProcessMouseScroll(float _yOffset)
{
    if (m_movementSpeed + _yOffset <= m_maxSpeed && m_movementSpeed + _yOffset >= m_minSpeed)
    {
        m_movementSpeed += (float)_yOffset;
    }
}

Matrix4 Camera::GetViewMatrix()
{
    return Matrix4::LookAt(position, position + m_front, m_up);
}

float Camera::GetZoom()
{
    return m_zoom;
}

float Camera::GetNear()
{
    return m_near;
}

float Camera::GetFar()
{
    return m_far;
}

Vector3 Camera::GetFront()
{
    return m_front;
}

Vector3 Camera::GetUp()
{
    return m_up;
}

Vector3 Camera::GetRight()
{
    return m_right;
}
