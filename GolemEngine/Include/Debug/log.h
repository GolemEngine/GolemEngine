#pragma once

#include <filesystem>
#include <fstream>

#include "dll.h"


class Log
{
private:
	static std::fstream m_file;

public:
	GOLEM_ENGINE_API Log() {};
	GOLEM_ENGINE_API ~Log();

	GOLEM_ENGINE_API static void OpenFile(std::filesystem::path const& _filename);
	GOLEM_ENGINE_API static void Print(const char* _format, ...);
};