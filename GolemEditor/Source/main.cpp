#include "editor.h"
#include <iostream>

extern "C"
{
	_declspec(dllexport) uint32_t NvOptimusEnablement = 1;
	_declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
}


int main()
{
	Editor* editor = new Editor();
	editor->Run();
	delete editor;
	std::cout << "Project Closed" << std::endl;
	return 0;
}