#pragma once

#include "UI/Windows/window.h"


class GolemEngine;
class GameObject;
class Terrain;

class SceneGraph : public Window
{
private:
	GameObject* m_renamingGameObject = nullptr;
	Terrain* m_renamingTerrain = nullptr;

public:
	SceneGraph(std::string _name);
	~SceneGraph();

	void Update() override;
	void DisplayObjects(GameObject* _gameObject);
};