#pragma once

#include <vector2.h>
#include <vector3.h>

#include "UI/Windows/window.h"


class WorldBuilderWindow : public Window
{
private:
#pragma region Create Default Terrain Variables
    bool m_isCreateDefaultTerrainPopupActive = false;
    int m_newDefaultTerrainResolutionX = 0;
    int m_newDefaultTerrainResolutionZ = 0;
#pragma endregion Create Default Terrain Variables
    
#pragma region Create Noisemap Terrain Variables
    bool m_isCreateDefaultNoisemapTerrainPopupActive = false;
    float m_newDefaultNoisemapTerrainAmplitude = 1.0f;
    std::string m_selectedHeightmap = "";
#pragma endregion Create Noisemap Terrain Variables
    
public:
    WorldBuilderWindow(std::string _name);
    ~WorldBuilderWindow();

    void Update() override;
    void UpdateCreateDefaultTerrainPopup(int& _xResolution, int& _zResolution);
    void UpdateCreateDefaultNoisemapeTerrainPopup();
};