#pragma once

#include "dll.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/sndfile.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Components/audio.h"

class GOLEM_ENGINE_API AudioWrapper
{
public:
    static AudioWrapper& GetInstance();

    bool InitDevice();

    ALCdevice* device;
    ALCcontext* context;
    ALfloat listenerPos[3] = { 0.0f, 0.0f, 0.0f };
    ALfloat listenerVel[3] = { 0.0f, 0.0f, 0.0f };

    void UpdatePosition();

private:
    AudioWrapper();
    ~AudioWrapper();
    AudioWrapper(const AudioWrapper& single) = delete; // Can not copy this class

    const AudioWrapper& operator=(const AudioWrapper& single) = delete;
};