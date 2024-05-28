#pragma once

#include "dll.h"

#include <string>


class Resource
{
public:
	GOLEM_ENGINE_API Resource() {};
	GOLEM_ENGINE_API virtual ~Resource() {};

	std::string path;
};