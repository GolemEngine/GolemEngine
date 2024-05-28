#pragma once


#include "dll.h"


class GOLEM_ENGINE_API Brush
{
public:
    bool isActive = false;
    float radius = 0.0f;
    float force = 0.0f;

public:
    Brush();

    void Update();
};
