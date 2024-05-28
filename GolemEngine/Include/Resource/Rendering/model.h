#pragma once

#include <vector>

#include "dll.h"
#include "Resource/resource.h"
#include "vertex.h"


class Model : public Resource
{
public:
	unsigned int VAO, VBO;
	std::vector<Vertex> vertices;

public:
	GOLEM_ENGINE_API Model();
	GOLEM_ENGINE_API ~Model();

	GOLEM_ENGINE_API void Load(const char* _modelPath);
	GOLEM_ENGINE_API void Load(std::vector<Vertex> _vertices);
};

