#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "UI/Windows/window.h"
#include "Wrappers/windowWrapper.h"


// Check the button state for realizing double click
struct ButtonState
{
	bool clicked = false;
	double lastClickTime = -1.0;
};

class FileBrowser : public Window
{
public:
	FileBrowser(std::string _name);
	~FileBrowser();

	void Update() override;

	void TreeNodes(std::filesystem::path _path);
	// Content Browser loop
	void ContentBrowser();
	void LastPath(std::filesystem::path& _currentPath);
	// Get ever file name
	const char* GetFolderName(const char* _path);
	void RightMouseClickEvent();
	void CreateFolder();
	void RenameFolder(std::string _folderPath);
	void DeleteFolder(const std::string& _folderPath);
	void LoadFile(const std::string& _filePath);
	
	std::string selectedFolder;
	std::string draggingFilePath;

	bool isRenaming = false;
	bool isLoadUi = false;
	bool isDragging = false;

	Golemint Ui_Default;
	Golemint Ui_Folder;
	Golemint Ui_Cpp;
	Golemint Ui_H;
	Golemint Ui_Obj;

	//std::vector<std::string> m_loadedTextures;
	std::vector<std::pair<std::string, Golemint>> m_loadedTextures;

	void DragDropEvent();

private:
	ButtonState m_buttonState;
	const std::string m_editorDirectory = std::filesystem::current_path().string().c_str();
	std::filesystem::path m_currentDirectory;
};