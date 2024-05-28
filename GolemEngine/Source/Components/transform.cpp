#include "Components/transform.h"
#include "golemEngine.h"
#include "Core/gameobject.h"
#include "Resource/sceneManager.h"
#include "Inputs/inputManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "MathsLib/utils.h"
#include "ImGuizmo.h"
#include <cmath>

Transform::Transform()
    : localPosition({0.0f, 0.0f, 0.0f}), rotation({0.0f, 0.0f, 0.0f}), scaling({1.0f, 1.0f, 1.0f})
{
    m_parent = SceneManager::GetCurrentScene()->GetWorld()->transform;
    m_localModel = Matrix4::TRS(Vector3(0, 0, 0), Quaternion::EulerToQuaternion(Vector3(0, 0, 0)), Vector3(1.0f, 1.0f, 1.0f));
    if (m_parent)
        m_parent->AddChild(this);
}

Transform::Transform(Transform* _parent)
    : m_parent(_parent), localPosition({0.0f, 0.0f, 0.0f}), rotation({0.0f, 0.0f, 0.0f}), scaling({1.0f, 1.0f, 1.0f})
{
    m_localModel = Matrix4::TRS(Vector3(0, 0, 0), Quaternion::EulerToQuaternion(Vector3(0, 0, 0)), Vector3(1.0f, 1.0f, 1.0f));
    if (m_parent)
        m_parent->AddChild(this);
}


Transform::Transform(Vector3 _position, Vector3 _rotation, Vector3 _scaling, Transform* _parent)
	: localPosition(_position), rotation(_rotation), scaling(_scaling), m_parent(_parent)
{
	m_localModel = Matrix4::TRS(_position, Quaternion::EulerToQuaternion(_rotation), _scaling);
    if (m_parent)
      m_parent->AddChild(this);
    
}

Transform::~Transform()
{
    owner->DeleteTransform(this);
}


void Transform::Update() {}

void Transform::UpdateSelfAndChilds()
{
    oldScaling = m_localModel.TrsToScaling();
    m_localModel = Matrix4::TRS(localPosition, Quaternion::EulerToQuaternion(rotation), scaling);

    if (m_parent)
    {
        m_globalModel = m_parent->m_globalModel * m_localModel;
    }
    else
    {
        m_globalModel = m_localModel;
    }

    for (int i = 0; i < m_children.size(); i++)
    {
        m_children[i]->UpdateSelfAndChilds();
    }
    globalPosition = m_globalModel.TrsToPosition();
}


void Transform::EditTransformGizmo()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    static ImGuizmo::OPERATION currentOperation(ImGuizmo::TRANSLATE);
    static ImGuizmo::MODE currentMode(ImGuizmo::WORLD);

    //select operation with inputs while is not used
    if (!ImGuizmo::IsUsing())
    {
        if (InputManager::IsKeyPressed(KEY_X))
        {
            currentOperation = ImGuizmo::SCALE;
        }

        else if (InputManager::IsKeyPressed(KEY_Z))
        {
            currentOperation = ImGuizmo::TRANSLATE;
        }

        else if (InputManager::IsKeyPressed(KEY_C))
        {
            currentOperation = ImGuizmo::ROTATE;
        }
    }

    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();

    auto camera = GolemEngine::GetCamera();

    Matrix4 objectTransform = GetGlobalModel().Transpose();

    float aspectRatio = windowWidth / windowHeight;
    float fov = DegToRad(camera->GetZoom());

    Matrix4 cameraProjection = Matrix4::Projection(fov, aspectRatio,
        camera->Camera::GetNear(), camera->Camera::GetFar()).Transpose();

    Matrix4 cameraView = camera->GetViewMatrix().Transpose();

    ImGuizmo::SetRect(450, -350,
        io.DisplaySize.x, io.DisplaySize.y);

    //set snap functionnality and snap value
    bool snap = InputManager::IsKeyPressed(KEY_LEFT_CTRL);
    float snapValue = 0.5f;
    float snapValues[3] = {snapValue, snapValue, snapValue};

    //create TRS matrix
    float mat[16];
    ImGuizmo::RecomposeMatrixFromComponents(&localPosition.x,
        &rotation.x, &scaling.x, mat);

    //used to manipulate the gizmos
    ImGuizmo::Manipulate(&cameraView.data[0][0],
        &cameraProjection.data[0][0], currentOperation, currentMode,
        mat, nullptr, snap ? snapValues : nullptr);

    //decompose the TRS matrix to get 3 vector3 : translation, rotation, scaling
    float newPos[3], newRot[3], newScale[3];
    ImGuizmo::DecomposeMatrixToComponents(mat, newPos, newRot, newScale);
    
    //set the new values to the selected object's transform and check if used to avoid gizmos disapearance
    if (ImGuizmo::IsUsing() && currentOperation == ImGuizmo::TRANSLATE)
    {
        localPosition = Vector3(newPos[0], newPos[1], newPos[2]);
    }

    else if (ImGuizmo::IsUsing() && currentOperation == ImGuizmo::SCALE)
    {
        scaling = Vector3(newScale[0], newScale[1], newScale[2]);
    }

    else if (ImGuizmo::IsUsing() && currentOperation == ImGuizmo::ROTATE)
    {
        rotation = Vector3(newRot[0], newRot[1], newRot[2]);
    }

}

void Transform::AddChild(Transform* const _t)
{
    m_children.push_back(_t);
    if (_t->m_parent && _t->m_parent != this)
        _t->m_parent->RemoveChild(_t);
    _t->m_parent = this;
}

void Transform::AddChildren(std::vector<Transform*> const _ts)
{
    for (Transform* t : _ts)
        AddChild(t);
}

void Transform::RemoveChild(Transform* const _t)
{
    std::erase(m_children, _t);
    _t->m_parent = SceneManager::GetCurrentScene()->GetWorld()->transform;
}

void Transform::SetParent(Transform* const _t)
{
    if (m_parent)
        m_parent->RemoveChild(this);
    m_parent = _t;
    _t->m_children.push_back(this);

    m_globalModel = _t->m_globalModel.Inverse() * m_globalModel;

    localPosition = m_globalModel.TrsToPosition();
    rotation = Vector3::QuaternionToEuler(m_globalModel.TrsToRotation());
    scaling = m_globalModel.TrsToScaling();
}

bool Transform::IsChildOf(Transform* const _parent)
{
    return (m_parent == _parent);
}

GOLEM_ENGINE_API Vector3 Transform::GetForward() const
{
    double yaw = rotation.y * (M_PI / 180.0f);
    double pitch = rotation.x * (M_PI / 180.0f);

    Vector3 forward;
    forward.x = (float)(cos(yaw) * cos(pitch));
    forward.y = (float)sin(pitch);
    forward.z = (float)(sin(yaw) * cos(pitch));

    return forward;
}

bool Transform::IsAParentOf(Transform* _t)
{
    while (_t->m_parent != nullptr)
    {
        if (_t->m_parent == this)
            return true;
        _t = _t->m_parent;
    }
    return false;
}

Transform* Transform::GetParent()
{
    return m_parent;
}

Matrix4 Transform::GetGlobalModel()
{
	return m_globalModel;
}

Matrix4 Transform::GetLocalModel()
{
	return m_localModel;
}

const std::vector<Transform*>& Transform::GetChildren() const
{
    return m_children;
}