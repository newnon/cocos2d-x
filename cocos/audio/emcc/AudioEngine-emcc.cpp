/**
 * @author cesarpachon
 */ 
#include <cstring>
#include "AudioEngine-emcc.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>

using namespace cocos2d;
using namespace cocos2d::experimental;

extern "C" {
    typedef void (*preload_audio_callback)(const char* path, bool result);
    typedef void (*play_audio_callback)(int audioId, bool result);
    typedef void (*end_audio_callback)(int audioId);
    
    // Methods implemented in AudioEngine.js
    void AudioEngine_init(play_audio_callback play_callback, end_audio_callback end_callback, preload_audio_callback preload_callback);
    
    void AudioEngine_preload(const char *);
    void AudioEngine_unload(const char *);
    int AudioEngine_play2d(const char *, int, float);
    
    void AudioEngine_setLoop(int, bool);
    void AudioEngine_setVolume(int, float);
    void AudioEngine_pause(int);
    void AudioEngine_resume(int);
    void AudioEngine_stop(int);
    int AudioEngine_getDuration(int);
    int AudioEngine_getCurrentTime(int);
    void AudioEngine_setCurrentTime(int, float);
    
    void AudioEngine_setUseFileExt(const char *);
    
    void AudioEngine_end();
};

AudioEngineImpl * g_AudioEngineImpl = nullptr;

AudioEngineImpl::AudioEngineImpl()
{
    AudioEngine_init(&AudioEngineImpl::onPlayCallback, &AudioEngineImpl::onEndCallback, &AudioEngineImpl::onPreloadCallback);
}

AudioEngineImpl::~AudioEngineImpl()
{
    AudioEngine_end();
}

bool AudioEngineImpl::init()
{
    g_AudioEngineImpl = this;
    return true;
};

void AudioEngineImpl::onPreloadCallback(const char* path, bool success)
{
    if(path && g_AudioEngineImpl)
    {
        auto callback = g_AudioEngineImpl->_preloadCallbacks.find(path);
        if (callback != g_AudioEngineImpl->_preloadCallbacks.end())
        {
            callback->second(success);
            g_AudioEngineImpl->_preloadCallbacks.erase(callback);
        }
    }
}

void AudioEngineImpl::onPlayCallback(int audioId, bool success)
{
    if (g_AudioEngineImpl)
    {
        if (AudioEngine::_audioIDInfoMap.find(audioId) != AudioEngine::_audioIDInfoMap.end())
        {
            AudioEngine::_audioIDInfoMap[audioId].state = success ? AudioEngine::AudioState::PLAYING : AudioEngine::AudioState::ERROR;
        }
    }
}

void AudioEngineImpl::onEndCallback(int audioId)
{
    if (g_AudioEngineImpl)
    {
        auto it = g_AudioEngineImpl->_finishCallbacks.find(audioId);
        if (it != g_AudioEngineImpl->_finishCallbacks.end())
        {
            g_AudioEngineImpl->_finishCallbacks.erase(it);
            AudioEngine::remove(audioId);
        }
    }
}

int AudioEngineImpl::play2d(const std::string &fileFullPath, bool loop, float volume)
{
    volume = volume > 1.0 ? 1.0 : volume;
    volume = volume < 0.0 ? 0.0 : volume;
    
    int ret = AudioEngine_play2d(fileFullPath.c_str(), loop, volume);
    return ret;
};

void AudioEngineImpl::setVolume(int audioId, float volume)
{
    // Ensure volume is between 0.0 and 1.0.
    volume = volume > 1.0 ? 1.0 : volume;
    volume = volume < 0.0 ? 0.0 : volume;
    AudioEngine_setVolume(audioId, volume);
};

void AudioEngineImpl::setLoop(int audioId, bool loop)
{
    AudioEngine_setLoop(audioId, loop);
};

bool AudioEngineImpl::pause(int audioId)
{
    AudioEngine_pause(audioId);
    return true;
};

bool AudioEngineImpl::resume(int audioId)
{
    AudioEngine_resume(audioId);
    return true;
};

bool AudioEngineImpl::stop(int audioId)
{
    AudioEngine_stop(audioId);
    return true;
};

void AudioEngineImpl::stopAll()
{
    for(const auto &pair: AudioEngine::_audioIDInfoMap)
        AudioEngine_stop(pair.first);
};

float AudioEngineImpl::getDuration(int audioId)
{
    float value = AudioEngine_getDuration(audioId);
    return value;
};

float AudioEngineImpl::getCurrentTime(int audioId)
{
    float value = AudioEngine_getCurrentTime(audioId);
    return value;
};

bool AudioEngineImpl::setCurrentTime(int audioId, float time)
{
    AudioEngine_setCurrentTime(audioId, time);
    return true;
};

void AudioEngineImpl::setFinishCallback(int audioId, const FinishCallback &callback)
{
    if (audioId != -1)
    {
        auto callbackIt = _finishCallbacks.find(audioId);
        if (callbackIt == _finishCallbacks.end())
        {
            _finishCallbacks[audioId] = callback;
        }
    }
};

void AudioEngineImpl::uncache(const std::string& path)
{
    _preloadCallbacks.erase(path);
};

void AudioEngineImpl::uncacheAll()
{
    for(const auto &pair: AudioEngine::_audioPathIDMap)
        AudioEngine_unload(pair.first.c_str());
    
    _finishCallbacks.clear();
    _preloadCallbacks.clear();
};

void AudioEngineImpl::preload(const std::string& filePath, const Callback &callback)
{
    if(callback)
    {
        auto callbackIt = _preloadCallbacks.find(filePath);
        if (callbackIt == _preloadCallbacks.end())
        {
            _preloadCallbacks[filePath] = callback;
        }
    }
    AudioEngine_preload(filePath.c_str());
};

void AudioEngineImpl::setUseFileExt(const std::vector<std::string> &exts)
{
    std::string transferExts;
    for (int i = 0; i < exts.size(); ++i)
    {
        transferExts += exts[i];
        if (i + 1 < exts.size())
            transferExts += ",";
    }
    
    AudioEngine_setUseFileExt(transferExts.c_str());
}

void AudioEngineImpl::update(float dt)
{
};

#endif
