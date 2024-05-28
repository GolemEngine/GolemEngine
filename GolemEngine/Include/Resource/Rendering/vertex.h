#pragma once

#include "vector3.h"
#include "vector2.h"


struct GOLEM_ENGINE_API Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 textureCoords;
};