#pragma once

#include "dll.h"

#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <vector3.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/sndfile.h>

#include "Refl/refl.hpp"
#include "component.h"

class Audio : public Component
{
public:
    GOLEM_ENGINE_API Audio();
    GOLEM_ENGINE_API Audio(std::string _fileName, bool _isLooping = false);
    GOLEM_ENGINE_API ~Audio();
    
    std::string musicPath;
    ALuint source;
    ALuint buffer;

    GOLEM_ENGINE_API bool LoadAudioResource(const char* _fileName, std::vector<char>& _data, ALenum& _format, ALsizei& _frequency);
    GOLEM_ENGINE_API bool SetUpAudio();
    GOLEM_ENGINE_API void SetLoop(bool _isLooping);
    GOLEM_ENGINE_API void SetVolume(int _volume);
    GOLEM_ENGINE_API void StopMusic(bool m_isPlaying);
    GOLEM_ENGINE_API void SetPositon();
    GOLEM_ENGINE_API void CleanUp();
    GOLEM_ENGINE_API void Play();

    GOLEM_ENGINE_API void Update() override;

private:
    bool m_isInit = false;
    float m_volume = 1.0f;
    bool m_isLooping = false;
    bool m_isPlaying;
    std::thread m_thread;
    Vector3 m_position;
    ALfloat m_sourcePos[3] = { 0.0f, 0.0f, 0.0f };
    ALfloat m_sourceVel[3] = { 0.0f, 0.0f, 0.0f };

    friend refl_impl::metadata::type_info__<Audio>;


    // Define serialization and deserialization functions manually because the
    // macro is not used due to the pointer member variable.
    GOLEM_ENGINE_API void ToJson(json& j) const
    {
        j = json
        {
            {"name", "audio"},
            {"data",
                {
                    {"musicPath", musicPath},
                    {"volume", m_volume},
                    {"isLooping", m_isLooping},
                    {"isPlaying", m_isPlaying},
                }
            }
        };

    }
};

REFL_AUTO(
    type(Audio),
    field(m_volume)
)