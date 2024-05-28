#pragma once

#include "dll.h"
#include "vector2.h"
#include "brush.h"


class GOLEM_ENGINE_API WorldBuilder
{
public:
    static inline Brush* brush = new Brush();
    static inline float texture1Level = 0.0f;
    static inline float texture2Level = 0.0f;
    
private:
    // Private constructors and destructor to make static class
    WorldBuilder() = delete;
    WorldBuilder(WorldBuilder& _other) = delete;
    WorldBuilder(WorldBuilder&& _other) = delete;
    WorldBuilder& operator=(WorldBuilder& _other) = delete;
    WorldBuilder& operator=(WorldBuilder&& _other) = delete;
    void operator=(const WorldBuilder&) = delete;
    ~WorldBuilder() = delete;

public:
    static void CreateDefaultTerrain(int _xResolution, int _zResolution);
    static void CreateNoisemapTerrain(const char* _noisemapPath);
};