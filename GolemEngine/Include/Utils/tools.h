#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "dll.h"


#define print(x) std::cout << x << std::endl

class Scene;

namespace Tools
{
	std::string GOLEM_ENGINE_API FindFile(const std::string& _file);
	std::string GOLEM_ENGINE_API FindFolder(const std::string& _folderName);
	// Get the file extentension like .obj .cpp .h ......
	std::string GOLEM_ENGINE_API GetFileExtension(const std::string& _fileName);
	std::string GOLEM_ENGINE_API RemoveExtension(const std::string& _fileName);

	int GOLEM_ENGINE_API GetFolderSize(std::filesystem::path _pathToFolder);
	std::vector<std::string> GOLEM_ENGINE_API GetFolderElementsNames(const std::filesystem::path _pathToFolder);

	// Get the string of a saved path because saving a path to json modifies it so we need to re write it correctly
	std::string GOLEM_ENGINE_API GetPathFromJsonString(std::string _savedJsonPath);

	// Check shader compile errors
	void GOLEM_ENGINE_API CheckCompileErrors(unsigned int _shader, std::string _type);

	std::string GOLEM_ENGINE_API GetFileNameFromPath(const std::string& _path);
}