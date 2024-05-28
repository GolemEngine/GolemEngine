#pragma once

#include "UI/Windows/window.h"


class ViewportToolsWindow : public Window
{
public:
    bool isWireframeViewActive = false;
    bool isTerrainGradientViewActive = false;
    
public:
    ViewportToolsWindow(std::string _name);
    ~ViewportToolsWindow();

    void Update() override;
};