#include <iostream>
#include <thread>
#include <chrono>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/sndfile.h>

#include "Components/audio.h"
#include "Resource/resourceManager.h"
#include "Utils/tools.h"
#include "Wrappers/audioWrapper.h"
#include "Core/gameobject.h"
#include "golemEngine.h"

Audio::Audio()
    :musicPath(Tools::FindFile("music_01.wav")),
    m_position(0, 0, 0),
    m_isLooping(true),
    m_isPlaying(false)
{
    SetUpAudio();
}

Audio::~Audio()
{
    CleanUp();
}

Audio::Audio(std::string _fileName, bool _isLooping)
    :musicPath(Tools::FindFile(_fileName)),
    m_position(0, 0, 0),
    m_isLooping(_isLooping),
    m_isPlaying(false)
{
    SetUpAudio();
}

bool Audio::LoadAudioResource(const char* _fileName, std::vector<char>& _data, ALenum& _format, ALsizei& _frequency)
{
    // Open the WAV file
    SF_INFO info;
    SNDFILE* file = sf_open(_fileName, SFM_READ, &info);
    if (!file) 
    {
        std::cerr << "Failed to open WAV file: " << _fileName << std::endl;
        return false;
    }

    // Check if the file format is supported
    if (info.channels != 1 && info.channels != 2) 
    {
        std::cerr << "Unsupported number of channels: " << info.channels << std::endl;
        sf_close(file);
        return false;
    }

    // Determine the OpenAL format based on the number of channels
    _format = (info.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    // Read the audio data into memory
    const sf_count_t frameCount = info.frames * info.channels;
    _data.resize(frameCount * sizeof(short));
    const sf_count_t readCount = sf_read_short(file, reinterpret_cast<short*>(_data.data()), frameCount);
    if (readCount != frameCount) 
    {
        std::cerr << "Failed to read WAV file data" << std::endl;
        sf_close(file);
        return false;
    }

    // Set the frequency
    _frequency = info.samplerate;

    // Close the file
    sf_close(file);

    return true;
}

bool Audio::SetUpAudio()
{
    alGenBuffers(1, &buffer);

    std::vector<char> data;
    ALenum format;
    ALsizei frequency;

    if (!LoadAudioResource(musicPath.c_str(), data, format, frequency))
    {
        std::cerr << "Failed to load WAV file" << std::endl;
        return false;
    }

    alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), frequency);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    alDistanceModel(AL_INVERSE_DISTANCE);

    if(m_isLooping)
        alSourcei(source, AL_LOOPING, AL_TRUE);

    return true;
}

void Audio::SetLoop(bool _isLooping)
{
    m_isLooping = _isLooping;
}

void Audio::SetVolume(int _volume)
{
    m_volume = (float)_volume;
}

void Audio::StopMusic(bool _isPlaying)
{
    m_isPlaying = _isPlaying;
}

void Audio::SetPositon()
{
    if (owner)
    {
        m_position = owner->transform->globalPosition;
        m_sourcePos[0] = m_position.x;
        m_sourcePos[1] = m_position.y;
        m_sourcePos[2] = m_position.z;
        alSourcefv(source, AL_POSITION, m_sourcePos);
        alSourcefv(source, AL_VELOCITY, m_sourceVel);
        alSourcei(source, AL_LOOPING, AL_TRUE);
        alSourcef(source, AL_MAX_DISTANCE, 10.0f);
    }
}

void Audio::CleanUp()
{
    //m_thread.join();
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}

void Audio::Play()
{
    m_isPlaying = true;
    //m_thread = std::thread(alSourcePlay, source);
    alSourcePlay(source);
}

void Audio::Update()
{
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);

    alSourcef(source, AL_GAIN, m_volume);
    SetPositon();

    if (!GolemEngine::GetGameMode())
    {
        if (state == AL_PLAYING)
        {
            alSourcePause(source);
        }
    }
    else
    {
        if (state != AL_PLAYING)
        {
            Play();
        }
    }

    if (!m_isPlaying)
    {
        CleanUp();
    }
}
