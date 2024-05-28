#pragma once

#include <string>


class Window
{
public:
	std::string name;

public:
	Window(std::string _name);
	virtual void Update() = 0;
};