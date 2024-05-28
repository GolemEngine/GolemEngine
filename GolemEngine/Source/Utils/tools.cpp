#include "Utils/tools.h"

#include <filesystem>
#include <string>
#include <iostream>
#include <stack>
#include <nlohmann/json.hpp>

#include "dll.h"
#include "Wrappers/graphicWrapper.h"

using json = nlohmann::json;


namespace Tools
{
    std::string GOLEM_ENGINE_API FindFile(const std::string& _file)
    {
        std::string targetFileName;
        for (const auto& file : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            if (file.path().filename().string().find(_file) != std::string::npos)
            {
                return file.path().string();
            }
        }
        std::cout << "Didn't find the file: " << _file << std::endl;
        return ""; // Return an empty string
    }
    
    std::string GOLEM_ENGINE_API FindFolder(const std::string& _folderName)
    {
        for (const auto& dir : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            if (dir.is_directory() && dir.path().filename().string().find(_folderName) != std::string::npos)
            {
                return dir.path().string();
            }
        }

        // If the folder is not found, output a message
        std::cout << "Not found the folder: " << _folderName << std::endl;
        return ""; // Return an empty string
    }

    std::string GOLEM_ENGINE_API GetFileExtension(const std::string& _fileName)
    {
        size_t dotPosition = _fileName.find_last_of('.');
        if (dotPosition != std::string::npos)
        {
            return _fileName.substr(dotPosition);
        }
        return "";
    }

    int GOLEM_ENGINE_API GetFolderSize(std::filesystem::path _pathToFolder)
    {
        int fileCount = 0;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(_pathToFolder))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                fileCount++;
            }
        }
        return fileCount;
    }

    std::vector<std::string> GetFolderElementsNames(const std::filesystem::path _pathToFolder)
    {
        std::vector<std::string> elements;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(_pathToFolder))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                elements.push_back(entry.path().filename().string());
            }
            else if (std::filesystem::is_directory(entry))
            {
                elements.push_back(entry.path().filename().string() + " [directory]");
            }
        }
        return elements;
    }

    std::string GOLEM_ENGINE_API RemoveExtension(const std::string& _filename)
    {
        size_t lastDot = _filename.find_last_of(".");
        if (lastDot != std::string::npos) {
            return _filename.substr(0, lastDot);
        }
        return _filename;
    }

    std::string GOLEM_ENGINE_API GetPathFromJsonString(std::string _savedJsonPath)
    {
        json p;
        p["path"] = _savedJsonPath;
        std::string jsonString = p.dump();
        json jParsed = json::parse(jsonString);
        std::string parsedPath = jParsed["path"];
        return parsedPath;
    }

    void GOLEM_ENGINE_API CheckCompileErrors(unsigned int _shader, std::string _type)
    {
        int success;
        char infoLog[1024];
        if (_type != "PROGRAM")
        {
            GraphicWrapper::GetShaderIv(_shader, SHADER_COMPILE_STATUS, &success);

            if (!success)
            {
                GraphicWrapper::GetShaderLog(_shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << _type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            GraphicWrapper::GetShaderIv(_shader, SHADER_LINK_STATUS, &success);
            if (!success)
            {
                GraphicWrapper::GetShaderLog(_shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << _type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    std::string GOLEM_ENGINE_API GetFileNameFromPath(const std::string& _path)
    {
        std::filesystem::path path(_path);
        return path.filename().string();
    }
}
