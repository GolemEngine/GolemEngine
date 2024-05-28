#pragma once

#include <filesystem>
#include <string>

#include "dll.h"
#include "Resource/resource.h"


class Texture : public Resource
{
public:
	unsigned int id = 0;
	unsigned int m_width;
	unsigned int m_height;
	std::string name;

public :
	GOLEM_ENGINE_API Texture();
	GOLEM_ENGINE_API Texture(unsigned int _width, unsigned int _height, unsigned int _format, unsigned int _internalFormat);
	GOLEM_ENGINE_API ~Texture();

	GOLEM_ENGINE_API void Load(const char* _filename);

	GOLEM_ENGINE_API void Use();
	GOLEM_ENGINE_API void UnUse();
};